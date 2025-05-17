#include <controller/DebugController.h>

#include <registration/input.h>

#include "../../../out/cmake-x64-release/_deps/sdl2-src/include/SDL_events.h"


namespace game
{
    DebugController::DebugController( )
    {
        try_register_device( engine::binding::DeviceType::KEYBOARD );
    }


    void DebugController::device_registered( engine::InputMappingContext& context, engine::binding::DeviceInfo deviceInfo )
    {
        PlayerController::device_registered( context, deviceInfo );

        printf( "DEBUG CONTROLLER: registered device at id %d\n", deviceInfo.id );

        context.bind_to_input_action( this, engine::UID( IA::QUIT ), &DebugController::quit,
                                      engine::binding::TriggerEvent::TRIGGERED );
    }


    void DebugController::quit( ) const
    {
        SDL_Event quitEvent{ .type = SDL_QUIT };
        SDL_PushEvent( &quitEvent );
    }

}
