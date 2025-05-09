#include <registration/audio.h>

#include <services/sound/ParallelSoundSystem.h>
#include <services/sound/SdlSoundSystem.h>
#include <services/sound/SoundSystemLogger.h>
#include <singletons/ServiceLocator.h>

#include <iostream>


using namespace engine;

namespace game
{
    void initialize_audio( )
    {
        auto underlyingService = std::make_unique<SdlSoundSystem>( 2u, sound::SdlInitInfo{}, sound::QueuePolicy::REPLACE_NEWEST );
        auto soundService      = std::make_unique<ParallelSoundSystem>( std::move( underlyingService ) );

#ifndef NDEBUG
        auto logger = std::make_unique<SoundSystemLogger>( std::move( soundService ), std::clog, std::cerr );
        SERVICE_LOCATOR.register_sound_system( std::move( logger ) );
#else
        SERVICE_LOCATOR.register_sound_system( std::move( soundService ) );
#endif
    }

}
