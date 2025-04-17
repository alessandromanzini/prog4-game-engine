#include <singletons/ServiceLocator.h>

#include <cassert>


namespace engine
{
    SoundSystem& ServiceLocator::get_sound_system( ) const
    {
        assert(sound_system_instance_ptr_ && "SoundSystem instance not registered");
        return *sound_system_instance_ptr_;
    }


    void ServiceLocator::register_sound_system( std::unique_ptr<SoundSystem>&& ss )
    {
        sound_system_instance_ptr_ = std::move( ss );
    }


}
