#include <controller/DebugController.h>

#include <registration/input.h>

#include <SDL.h>
#include <component/LevelLoaderComponent.h>
#include <registration/tags.h>
#include <singleton/ScenePool.h>
#include <singleton/ServiceLocator.h>


namespace game
{
    DebugController::DebugController( )
    {
        try_register_device( engine::binding::DeviceType::KEYBOARD );
    }


    void DebugController::device_registered( engine::InputMappingContext& context, const engine::binding::DeviceInfo deviceInfo )
    {
        PlayerController::device_registered( context, deviceInfo );

        printf( "DEBUG CONTROLLER: registered device at id %d\n", deviceInfo.id );

        context.bind_to_input_action( this, engine::UID( IA::SKIP_LEVEL ), &DebugController::skip_level,
                                      engine::binding::TriggerEvent::TRIGGERED );
        context.bind_to_input_action( this, engine::UID( IA::TOGGLE_MUTE ), &DebugController::toggle_mute,
                                      engine::binding::TriggerEvent::TRIGGERED );
        context.bind_to_input_action( this, engine::UID( IA::QUIT ), &DebugController::quit,
                                      engine::binding::TriggerEvent::TRIGGERED );
    }


    void DebugController::skip_level( ) const
    {
        const auto loader = engine::SCENE_POOL.get_active_scene( ).find_object(
            []( const engine::GameObject& object ) -> bool
                {
                    return object.get_tag( ) == engine::UID( ObjectTags::LEVEL_LOADER );
                } );
        if ( loader )
        {
            loader.value(  ).get_component<LevelLoaderComponent>( ).value( ).destroy_all_enemies( );
        }
    }


    void DebugController::toggle_mute( ) const
    {
        auto& ss = engine::SERVICE_LOCATOR.get_sound_system( );
        ss.set_master_volume( ss.get_master_volume( ) == 0.f ? 1.f : 0.f );
    }


    void DebugController::quit( ) const
    {
        SDL_Event quitEvent{ .type = SDL_QUIT };
        SDL_PushEvent( &quitEvent );
    }

}
