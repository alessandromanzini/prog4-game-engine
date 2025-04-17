#ifndef SERVICELOCATOR_H
#define SERVICELOCATOR_H

#include <services/sound/SoundSystem.h>
#include <singletons/Singleton.h>

#include <memory>


namespace engine
{
    class ServiceLocator final : public Singleton<ServiceLocator>
    {
        friend class Singleton;
    public:
        ~ServiceLocator( ) override = default;

        ServiceLocator( const ServiceLocator& )                = delete;
        ServiceLocator( ServiceLocator&& ) noexcept            = delete;
        ServiceLocator& operator=( const ServiceLocator& )     = delete;
        ServiceLocator& operator=( ServiceLocator&& ) noexcept = delete;

        SoundSystem& get_sound_system() const;
        void register_sound_system( std::unique_ptr<SoundSystem>&& ss );

    private:
        std::unique_ptr<SoundSystem> sound_system_instance_ptr_{};

        ServiceLocator( )  = default;

    };

}


#endif //!SERVICELOCATOR_H
