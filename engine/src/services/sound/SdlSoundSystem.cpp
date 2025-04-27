#include <services/sound/SdlSoundSystem.h>

#include <framework/threading.h>
#include <singletons/ResourceManager.h>

#include <framework/resources/sound/SdlAudio.h>

#include <SDL_mixer.h>

#include <cassert>
#include <queue>
#include <thread>
#include <string_view>


namespace engine
{
    namespace sound
    {
        using sound_pointer_t = std::variant<Mix_Chunk*, Mix_Music*>;

        struct SoundPlaybackOptions final
        {
            const Audio* audio{ nullptr };
            float volume{ 0.f };
            int loops{ 0 };
        };

        struct SoundInstance final
        {
            std::shared_ptr<SdlAudio> instance{ nullptr };
            bool queued{ false };
            bool playing{ false };
            bool paused{ false };
            uint8_t channel{};
        };
    }

    // +---------------------------+
    // | PIMPL SIGNATURE           |
    // +---------------------------+
    class SdlSoundSystem::SdlSoundSystemImpl final
    {
    public:
        explicit SdlSoundSystemImpl( uint8_t channels, sound::SdlInitInfo info, sound::QueuePolicy policy );
        ~SdlSoundSystemImpl( );

        SdlSoundSystemImpl( const SdlSoundSystemImpl& )                = delete;
        SdlSoundSystemImpl( SdlSoundSystemImpl&& ) noexcept            = delete;
        SdlSoundSystemImpl& operator=( const SdlSoundSystemImpl& )     = delete;
        SdlSoundSystemImpl& operator=( SdlSoundSystemImpl&& ) noexcept = delete;

        std::shared_ptr<Audio> load_sound( const std::filesystem::path& path, sound::SoundType type, UID tagId );
        void unload_unused_resources( );

        void play( const Audio& audio, float volume, int loops );

        bool stop( const Audio& audio );
        void stop_all( );

        bool pause( const Audio& audio );
        bool resume( const Audio& audio );

        [[nodiscard]] bool is_playing( const Audio& audio ) const;
        [[nodiscard]] bool is_paused( const Audio& audio ) const;

        [[nodiscard]] const Audio* get_current_track( ) const;

        void set_master_volume( float volume );
        [[nodiscard]] float get_master_volume( ) const;

        void set_volume_by_tag( UID tagId, float volume );
        [[nodiscard]] float get_volume_by_tag( UID tagId ) const;

        void process_requests( );

    private:
        static constexpr uint8_t MAX_CHANNELS_{ 16 };
        static constexpr uint16_t WAIT_INTERVAL_MS_{ 10 };

        const uint8_t channels_{};
        const sound::QueuePolicy queue_policy_{ sound::QueuePolicy::DISCARD };

        // TODO: Create wrapper for these
        threading::SafeResource<std::unordered_map<UID, sound::SoundInstance>> sound_resources_;
        threading::SafeResource<std::deque<uint8_t>> used_channel_queue_;
        std::vector<std::optional<UID>> channels_pool_;

        std::queue<sound::SoundPlaybackOptions> pending_queue_{};

        sound::SoundInstance* current_track_ptr_{};

        float master_volume_{ 1.f };
        std::unordered_map<UID, float> tag_volumes_{};

        void assert_on_missing_sound( const Audio& audio ) const;
        void assert_on_missing_tag( UID tagId ) const;

        [[nodiscard]] int find_open_channel( ) const;
        void wait_for_channel( uint8_t channel ) const;
        void wait_for_audio( const Audio& audio ) const;

        void process_sound_effect( uint8_t channel, UID soundId, Mix_Chunk* effect, int mixVolume, int loops );
        void process_sound_track( UID soundId, Mix_Music* track, int mixVolume, int loops );

        bool is_sound_in_queue( const Audio& audio ) const;

        bool halt_effect( UID soundId );
        bool halt_track( );

        bool pause_effect( UID soundId );
        bool pause_track( );

        bool resume_effect( UID soundId );
        bool resume_track( );

    };


    // +---------------------------+
    // | PIMPL IMPL                |
    // +---------------------------+
    SdlSoundSystem::SdlSoundSystemImpl::SdlSoundSystemImpl( const uint8_t channels, const sound::SdlInitInfo info,
                                                            const sound::QueuePolicy policy )
        : channels_{ channels }
        , queue_policy_{ policy }
        , sound_resources_{ decltype(sound_resources_)::resource_t{} }
        , used_channel_queue_{ decltype(used_channel_queue_)::resource_t( channels ) }
        , channels_pool_( channels )
    {
        assert( channels_ <= MAX_CHANNELS_ && "Too many channels requested!" );

        Mix_OpenAudio( static_cast<int>( info.sampleRate ),
                       MIX_DEFAULT_FORMAT,
                       static_cast<int>( info.channelType ),
                       info.bufferSize );
        Mix_Init( MIX_INIT_WAVPACK | MIX_INIT_MP3 | MIX_INIT_FLAC );
        Mix_AllocateChannels( channels_ );
    }


    SdlSoundSystem::SdlSoundSystemImpl::~SdlSoundSystemImpl( )
    {
        stop_all( );
        Mix_CloseAudio( );
        Mix_Quit( );
    }


    std::shared_ptr<Audio> SdlSoundSystem::SdlSoundSystemImpl::load_sound( const std::filesystem::path& path,
                                                                           const sound::SoundType type,
                                                                           const UID tagId )
    {
        const UID soundId{ path.string( ) };

        // Check if the sound is already registered. If not we can register it
        if ( not sound_resources_.cget( ).contains( soundId ) )
        {
            auto [lock, resources] = sound_resources_.get( );
            resources[soundId]     = {
                std::make_shared<SdlAudio>( RESOURCE_MANAGER.get_data_path( ) / path,
                                            type, soundId, tagId )
            };
        }
        else
        {
            assert(
                sound_resources_.cget( ).at( soundId ).instance->get_tag_id( ) == tagId &&
                "Sound already registered with a different tag!" );
        }

        // Initialize the tag volume to 1.0f if it doesn't exist
        if ( not tag_volumes_.contains( tagId ) )
        {
            tag_volumes_[tagId] = 1.f;
        }

        return sound_resources_.cget( ).at( soundId ).instance;
    }


    void SdlSoundSystem::SdlSoundSystemImpl::unload_unused_resources( )
    {
        auto [lock, resources] = sound_resources_.get( );
        for ( auto it = resources.begin( ); it != resources.end( ); )
        {
            if ( auto& instance = it->second.instance; instance.use_count( ) == 1 )
            {
                if ( is_sound_in_queue( *instance ) )
                {
                    // If sound is in queue, we defer until next frame
                    RESOURCE_MANAGER.signal_lifetime_event( event::LifetimeEvent::UNLOAD_AUDIO );
                }
                else if ( is_playing( *instance ) )
                {
                    // If the sound is still being played, we defer until playback is finished
                    std::thread{
                        [&, this]( const Audio& audio )
                            {
                                wait_for_audio( audio );
                                RESOURCE_MANAGER.signal_lifetime_event( event::LifetimeEvent::UNLOAD_AUDIO );
                            },
                        std::ref( *instance )
                    }.detach( );
                }
                else
                {
                    it = resources.erase( it );
                    continue;
                }
            }
            ++it;
        }
    }


    void SdlSoundSystem::SdlSoundSystemImpl::play( const Audio& audio, const float volume, const int loops )
    {
        assert_on_missing_sound( audio );

        const float tagVolume{ tag_volumes_.at( audio.get_tag_id( ) ) };
        pending_queue_.emplace( &audio, volume * master_volume_ * tagVolume, loops );

        auto [lock, resources]                       = sound_resources_.get( );
        resources.at( audio.get_sound_id( ) ).queued = true;
    }


    bool SdlSoundSystem::SdlSoundSystemImpl::stop( const Audio& audio )
    {
        assert_on_missing_sound( audio );

        switch ( audio.get_type( ) )
        {
            case sound::SoundType::SOUND_EFFECT:
                return halt_effect( audio.get_sound_id( ) );

            case sound::SoundType::SOUND_TRACK:
                return halt_track( );
        }
        return false;
    }


    void SdlSoundSystem::SdlSoundSystemImpl::stop_all( )
    {
        halt_track( );
        Mix_HaltChannel( -1 );

        for ( auto& soundId : channels_pool_ )
        {
            if ( soundId.has_value( ) )
            {
                halt_effect( soundId.value( ) );
            }
        }
    }


    bool SdlSoundSystem::SdlSoundSystemImpl::pause( const Audio& audio )
    {
        assert_on_missing_sound( audio );

        switch ( audio.get_type( ) )
        {
            case sound::SoundType::SOUND_EFFECT:
                return pause_effect( audio.get_sound_id( ) );

            case sound::SoundType::SOUND_TRACK:
                return pause_track( );
        }
        return false;
    }


    bool SdlSoundSystem::SdlSoundSystemImpl::resume( const Audio& audio )
    {
        assert_on_missing_sound( audio );

        switch ( audio.get_type( ) )
        {
            case sound::SoundType::SOUND_EFFECT:
                return resume_effect( audio.get_sound_id( ) );

            case sound::SoundType::SOUND_TRACK:
                return resume_track( );
        }
        return false;
    }


    bool SdlSoundSystem::SdlSoundSystemImpl::is_playing( const Audio& audio ) const
    {
        assert_on_missing_sound( audio );

        switch ( audio.get_type( ) )
        {
            case sound::SoundType::SOUND_EFFECT:
                return sound_resources_.cget( ).at( audio.get_sound_id( ) ).playing;

            case sound::SoundType::SOUND_TRACK:
                return Mix_PlayingMusic( );
        }
        return false;
    }


    bool SdlSoundSystem::SdlSoundSystemImpl::is_paused( const Audio& audio ) const
    {
        assert_on_missing_sound( audio );

        switch ( audio.get_type( ) )
        {
            case sound::SoundType::SOUND_EFFECT:
                return sound_resources_.cget( ).at( audio.get_sound_id( ) ).paused;

            case sound::SoundType::SOUND_TRACK:
                return Mix_PausedMusic( );
        }
        return false;
    }


    const Audio* SdlSoundSystem::SdlSoundSystemImpl::get_current_track( ) const
    {
        if ( not current_track_ptr_ )
        {
            return nullptr;
        }
        return current_track_ptr_->instance.get( );
    }


    void SdlSoundSystem::SdlSoundSystemImpl::set_master_volume( const float volume )
    {
        master_volume_ = std::clamp( volume, 0.f, 1.f );
    }


    float SdlSoundSystem::SdlSoundSystemImpl::get_master_volume( ) const
    {
        return master_volume_;
    }


    void SdlSoundSystem::SdlSoundSystemImpl::set_volume_by_tag( const UID tagId, const float volume )
    {
        tag_volumes_.at( tagId ) = std::clamp( volume, 0.f, 1.f );
    }


    float SdlSoundSystem::SdlSoundSystemImpl::get_volume_by_tag( const UID tagId ) const
    {
        assert_on_missing_tag( tagId );
        return tag_volumes_.at( tagId );
    }


    void SdlSoundSystem::SdlSoundSystemImpl::process_requests( )
    {
        while ( not pending_queue_.empty( ) )
        {
            auto& [requested, volume, loops]{ pending_queue_.front( ) };

            auto [lock, resources] = sound_resources_.get( );
            auto& resource         = resources.at( requested->get_sound_id( ) );
            auto& audio            = *resource.instance;

            const int mixVolume{ static_cast<int>( MIX_MAX_VOLUME * volume ) };

            switch ( audio.get_type( ) )
            {
                case sound::SoundType::SOUND_EFFECT:
                    if ( const auto channel{ find_open_channel( ) }; channel != -1 )
                    {
                        std::thread{
                            &SdlSoundSystemImpl::process_sound_effect, this, static_cast<uint8_t>( channel ), audio.get_sound_id( ),
                            std::get<Mix_Chunk*>( audio.get_resource( ) ), mixVolume, loops
                        }.detach( );
                    }
                    break;
                case sound::SoundType::SOUND_TRACK:
                {
                    process_sound_track( audio.get_sound_id( ), std::get<Mix_Music*>( audio.get_resource( ) ), mixVolume, loops );
                    break;
                }
            }

            resource.queued = false;
            pending_queue_.pop( );
        }
    }


    void SdlSoundSystem::SdlSoundSystemImpl::assert_on_missing_sound( [[maybe_unused]] const Audio& audio ) const
    {
        assert( sound_resources_.cget( ).contains( audio.get_sound_id( ) ) && "Sound not registered!" );
    }


    void SdlSoundSystem::SdlSoundSystemImpl::assert_on_missing_tag( [[maybe_unused]] const UID tagId ) const
    {
        assert( tag_volumes_.contains( tagId ) && "Tag not registered!" );
    }


    int SdlSoundSystem::SdlSoundSystemImpl::find_open_channel( ) const
    {
        // Find the first available channel
        for ( size_t channel{}; channel < channels_; ++channel )
        {
            if ( not Mix_Playing( static_cast<int>( channel ) ) )
            {
                return static_cast<int>( channel );
            }
        }

        // If not available, we need to replace one according to the policy
        switch ( queue_policy_ )
        {
            case sound::QueuePolicy::REPLACE_OLDEST:
                return used_channel_queue_.cget( ).back( );

            case sound::QueuePolicy::REPLACE_NEWEST:
                return used_channel_queue_.cget( ).front( );

            default:
            case sound::QueuePolicy::DISCARD:
                return -1;
        }
    }


    void SdlSoundSystem::SdlSoundSystemImpl::wait_for_channel( const uint8_t channel ) const
    {
        while ( Mix_Playing( channel ) )
        {
            std::this_thread::sleep_for( std::chrono::milliseconds( WAIT_INTERVAL_MS_ ) );
        }
    }


    void SdlSoundSystem::SdlSoundSystemImpl::wait_for_audio( const Audio& audio ) const
    {
        while ( is_playing( audio ) )
        {
            std::this_thread::sleep_for( std::chrono::milliseconds( WAIT_INTERVAL_MS_ ) );
        }
    }


    void SdlSoundSystem::SdlSoundSystemImpl::process_sound_effect( const uint8_t channel, const UID soundId, Mix_Chunk* effect,
                                                                   const int mixVolume, const int loops )
    {
        Mix_VolumeChunk( effect, mixVolume );

        // ReSharper disable once CppDFANotInitializedField
        if ( const auto result = Mix_PlayChannel( channel, effect, loops ); result == -1 )
        {
            throw std::runtime_error( SDL_GetError( ) );
        }
        else if ( result != channel )
        {
            throw std::runtime_error( "Channel mismatch!" );
        }

        // Set all playback infos
        {
            auto [queueLock, queue] = used_channel_queue_.get( );
            queue.push_front( channel );


            auto [resourceLock, resources] = sound_resources_.get( );
            auto& resource                 = resources.at( soundId );
            resource.playing               = true;
            resource.paused                = true;
            resource.channel               = channel;
        }

        wait_for_channel( channel );

        // Release all playback infos
        {
            auto [queueLock, queue] = used_channel_queue_.get( );
            std::erase( queue, channel );

            auto [resourceLock, resources] = sound_resources_.get( );
            auto& resource                 = resources.at( soundId );
            resource.playing               = false;
            resource.paused                = false;
        }
    }


    void SdlSoundSystem::SdlSoundSystemImpl::process_sound_track( const UID soundId, Mix_Music* track, const int mixVolume,
                                                                  const int loops )
    {
        // Stop previous track
        halt_track( );

        Mix_VolumeMusic( mixVolume );
        Mix_PlayMusic( track, loops );

        auto [lock, resources] = sound_resources_.get( );
        auto& resource         = resources.at( soundId );
        resource.playing       = true;
        resource.paused        = false;
        current_track_ptr_     = &resource;
    }


    bool SdlSoundSystem::SdlSoundSystemImpl::is_sound_in_queue( const Audio& audio ) const
    {
        return sound_resources_.cget( ).at( audio.get_sound_id( ) ).queued;
    }


    bool SdlSoundSystem::SdlSoundSystemImpl::halt_effect( const UID soundId )
    {
        auto [lock, resources] = sound_resources_.get( );
        auto& resource         = resources.at( soundId );

        if ( not resource.playing )
        {
            return false;
        }

        resource.playing                      = false;
        channels_pool_.at( resource.channel ) = std::nullopt;
        return true;
    }


    bool SdlSoundSystem::SdlSoundSystemImpl::halt_track( )
    {
        if ( not current_track_ptr_ )
        {
            return false;
        }

        Mix_HaltMusic( );
        current_track_ptr_ = nullptr;
        return true;
    }


    bool SdlSoundSystem::SdlSoundSystemImpl::pause_effect( const UID soundId )
    {
        auto [lock, resources] = sound_resources_.get( );
        auto& resource         = resources.at( soundId );

        if ( not resource.playing || resource.paused )
        {
            return false;
        }

        Mix_Pause( resource.channel );
        resource.paused = true;
        return true;
    }


    bool SdlSoundSystem::SdlSoundSystemImpl::pause_track( )
    {
        if ( not current_track_ptr_ || current_track_ptr_->paused )
        {
            return false;
        }

        Mix_PauseMusic( );
        current_track_ptr_->paused = true;
        return true;
    }


    bool SdlSoundSystem::SdlSoundSystemImpl::resume_effect( const UID soundId )
    {
        if ( not sound_resources_.cget( ).at( soundId ).paused )
        {
            return false;
        }

        auto [lock, resources] = sound_resources_.get( );
        auto& resource         = resources.at( soundId );
        Mix_Resume( resource.channel );
        resource.paused = false;
        return true;
    }


    bool SdlSoundSystem::SdlSoundSystemImpl::resume_track( )
    {
        if ( not current_track_ptr_ || not current_track_ptr_->paused )
        {
            return false;
        }

        Mix_ResumeMusic( );
        current_track_ptr_->paused = false;
        return true;
    }


    // +---------------------------+
    // | PIMPL PASSTHROUGH         |
    // +---------------------------+
    SdlSoundSystem::SdlSoundSystem( uint8_t channels, sound::SdlInitInfo info, sound::QueuePolicy policy )
        : impl_ptr_{ std::make_unique<SdlSoundSystemImpl>( channels, info, policy ) }
    {
        RESOURCE_MANAGER.add_lifetime_observer( *this );
    }


    SdlSoundSystem::~SdlSoundSystem( ) = default;


    ServiceType SdlSoundSystem::get_service_type( )
    {
        return ServiceType::SDL;
    }


    std::shared_ptr<Audio> SdlSoundSystem::load_sound( const std::filesystem::path& path,
                                                       const sound::SoundType type,
                                                       const UID tagId )
    {
        return impl_ptr_->load_sound( path, type, tagId );
    }


    void SdlSoundSystem::play( const Audio& audio, const float volume, const int loops )
    {
        impl_ptr_->play( audio, volume, loops );
    }


    bool SdlSoundSystem::stop( const Audio& audio )
    {
        return impl_ptr_->stop( audio );
    }


    void SdlSoundSystem::stop_all( )
    {
        impl_ptr_->stop_all( );
    }


    bool SdlSoundSystem::pause( const Audio& audio )
    {
        return impl_ptr_->pause( audio );
    }


    bool SdlSoundSystem::resume( const Audio& audio )
    {
        return impl_ptr_->resume( audio );
    }


    bool SdlSoundSystem::is_playing( const Audio& audio ) const
    {
        return impl_ptr_->is_playing( audio );
    }


    bool SdlSoundSystem::is_paused( const Audio& audio ) const
    {
        return impl_ptr_->is_paused( audio );
    }


    const Audio* SdlSoundSystem::get_current_track( ) const
    {
        return impl_ptr_->get_current_track( );
    }


    void SdlSoundSystem::set_master_volume( const float volume )
    {
        impl_ptr_->set_master_volume( volume );
    }


    float SdlSoundSystem::get_master_volume( ) const
    {
        return impl_ptr_->get_master_volume( );
    }


    void SdlSoundSystem::set_volume_by_tag( const UID tagId, const float volume )
    {
        impl_ptr_->set_volume_by_tag( tagId, volume );
    }


    float SdlSoundSystem::get_volume_by_tag( const UID tagId ) const
    {
        return impl_ptr_->get_volume_by_tag( tagId );
    }


    void SdlSoundSystem::process_requests( )
    {
        impl_ptr_->process_requests( );
    }


    void SdlSoundSystem::notify( const UID event, Subject* /* subject */, event::broadcast_value_variant_t /* value */ )
    {
        if ( event == UID( event::LifetimeEvent::UNLOAD_ALL ) ||
             event == UID( event::LifetimeEvent::UNLOAD_AUDIO ) )
        {
            impl_ptr_->unload_unused_resources( );
        }
    }

}
