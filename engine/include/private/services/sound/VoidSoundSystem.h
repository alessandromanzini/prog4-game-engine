#ifndef VOIDSOUNDSYSTEM_H
#define VOIDSOUNDSYSTEM_H

#include <services/sound/SoundSystem.h>


namespace engine
{
    class VoidSoundSystem final : public SoundSystem
    {
    public:
        [[nodiscard]] ServiceType get_service_type( ) override { return ServiceType::VOID; }


        [[nodiscard]] std::shared_ptr<Audio> load_sound( const std::filesystem::path&, sound::SoundType, UID ) override
        {
            return nullptr;
        }


        void play( const Audio&, float, int ) override { }

        bool stop( const Audio& ) override { return false; }
        void stop_all( ) override { }

        bool pause( const Audio& ) override { return false; }
        bool resume( const Audio& ) override { return false; }

        [[nodiscard]] bool is_playing( const Audio& ) const override { return false; }
        [[nodiscard]] bool is_paused( const Audio& ) const override { return false; }

        [[nodiscard]] const Audio* get_current_track( ) const override { return nullptr; }

        void set_master_volume( float ) override { }
        [[nodiscard]] float get_master_volume( ) const override { return 0.f; }

        void set_volume_by_tag( UID, float ) override { }
        [[nodiscard]] float get_volume_by_tag( UID ) const override { return 0.f; }

        void process_requests( ) override { }

    };

}


#endif //!VOIDSOUNDSYSTEM_H
