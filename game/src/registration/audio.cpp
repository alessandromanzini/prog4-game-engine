#include <registration/audio.h>

#include <services/sound/SdlSoundSystem.h>
#include <services/sound/SoundSystemLogger.h>
#include <singletons/ServiceLocator.h>

#include <iostream>


using namespace engine;

namespace game
{
    void initialize_audio( )
    {
        auto ss = std::make_unique<SdlSoundSystem>( static_cast<uint8_t>( 2 ), sound::SdlInitInfo{},
                                                    sound::QueuePolicy::REPLACE_NEWEST );
        auto logger = std::make_unique<SoundSystemLogger>( std::move( ss ), std::clog, std::cerr );

        SERVICE_LOCATOR.register_sound_system( std::move( logger ) );
    }

}
