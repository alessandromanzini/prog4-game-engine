#ifndef SDLSOUNDSYSTEM_H
#define SDLSOUNDSYSTEM_H

#include <event/Observer.h>
#include <services/sound/SoundSystem.h>

#include <cstdint>


namespace engine
{
    namespace sound
    {
        struct SdlInitInfo final
        {
            SampleRate sampleRate{ SampleRate::HZ_44100 };
            ChannelType channelType{ ChannelType::STEREO };
            uint16_t bufferSize{ 2048 };
        };
    }

    class SdlSoundSystem final : public SoundSystem, public Observer
    {
        class SdlSoundSystemImpl;

    public:
        explicit SdlSoundSystem( uint8_t channels, sound::SdlInitInfo info,
                                 sound::QueuePolicy policy = sound::QueuePolicy::DISCARD );
        ~SdlSoundSystem( ) override;

        SdlSoundSystem( const SdlSoundSystem& )                = delete;
        SdlSoundSystem( SdlSoundSystem&& ) noexcept            = delete;
        SdlSoundSystem& operator=( const SdlSoundSystem& )     = delete;
        SdlSoundSystem& operator=( SdlSoundSystem&& ) noexcept = delete;

        [[nodiscard]] ServiceType get_service_type( ) override;

        [[nodiscard]] std::shared_ptr<Audio> load_sound( const std::filesystem::path& path, sound::SoundType type, UID tagId ) override;

        void play( const Audio& audio, float volume, int loops ) override;

        bool stop( const Audio& audio ) override;
        void stop_all( ) override;

        bool pause( const Audio& audio ) override;
        bool resume( const Audio& audio ) override;

        [[nodiscard]] bool is_playing( const Audio& audio ) const override;
        [[nodiscard]] bool is_paused( const Audio& audio ) const override;

        [[nodiscard]] const Audio* get_current_track( ) const override;

        void set_master_volume( float volume ) override;
        [[nodiscard]] float get_master_volume( ) const override;

        void set_volume_by_tag( UID tagId, float volume ) override;
        [[nodiscard]] float get_volume_by_tag( UID tagId ) const override;

        void process_requests( ) override;

        void notify( UID event, Subject* subject, event::broadcast_value_variant_t value ) override;

    private:
        std::unique_ptr<SdlSoundSystemImpl> impl_ptr_{ nullptr };

    };

}


#endif //!SDLSOUNDSYSTEM_H
