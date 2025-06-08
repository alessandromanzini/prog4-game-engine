#include <registration/audio.h>

#include <service/sound/ParallelSoundSystem.h>
#include <service/sound/SdlSoundSystem.h>
#include <service/sound/SoundSystemLogger.h>
#include <singleton/ServiceLocator.h>

#include <iostream>


using namespace engine;

namespace game
{
    void initialize_audio( )
    {
        auto underlyingService = std::make_unique<SdlSoundSystem>( static_cast<uint8_t>( 8u ), sound::SdlInitInfo{}, sound::QueuePolicy::REPLACE_NEWEST );
        auto soundService      = std::make_unique<ParallelSoundSystem>( std::move( underlyingService ) );

#ifndef NDEBUG
        auto logger = std::make_unique<SoundSystemLogger>( std::move( soundService ), std::clog, std::cerr );
        SERVICE_LOCATOR.register_sound_system( std::move( logger ) );
#else
        SERVICE_LOCATOR.register_sound_system( std::move( soundService ) );
#endif
    }

}
