#include <controllers/TestController.h>

#include <components/MoveComponent.h>
#include <framework/components/AudioComponent.h>
#include <registration/input.h>

#include <iostream>

#include <SDL.h>
#include <singletons/ServiceLocator.h>


namespace game
{
    TestController::TestController( )
    {
        try_register_device( engine::binding::DeviceType::KEYBOARD );
    }


    void TestController::possess( engine::GameObject* pawn )
    {
        PlayerController::possess( pawn );

        audio_ptr_ = &pawn->get_component<engine::AudioComponent>( );
        move_ptr_  = &pawn->get_component<MoveComponent>( );

        assert( audio_ptr_ && move_ptr_ && "TestController::possess: Missing components" );
    }


    void TestController::device_registered( engine::InputMappingContext& context, const engine::binding::DeviceInfo deviceInfo )
    {
        std::cout << "Test device registered at id " << static_cast<int>( deviceInfo.id ) << '\n';

        context.bind_to_input_action( *this, engine::UID( IA::MOVE ), &TestController::move,
                                      engine::binding::TriggerEvent::PRESSED );

        context.bind_to_input_action( *this, engine::UID( IA::PLAY_SOUND ), &TestController::play_sound,
                                      engine::binding::TriggerEvent::TRIGGERED );

        context.bind_to_input_action( *this, engine::UID( IA::VOLUME_UP ), &TestController::volume_up,
                                      engine::binding::TriggerEvent::PRESSED );

        context.bind_to_input_action( *this, engine::UID( IA::VOLUME_DOWN ), &TestController::volume_down,
                                      engine::binding::TriggerEvent::PRESSED );

        context.bind_to_input_action( *this, engine::UID( IA::KILL ), &TestController::kill,
                                      engine::binding::TriggerEvent::TRIGGERED );

        context.bind_to_input_action( *this, engine::UID( IA::QUIT ), &TestController::quit,
                                      engine::binding::TriggerEvent::TRIGGERED );
    }


    void TestController::move( const glm::vec2 dir ) const
    {
        if ( not has_pawn(  ) )
        {
            return;
        }

        if ( not( dir.x == 0 && dir.y == 0 ) )
        {
            move_ptr_->move( dir );
        }
    }


    void TestController::play_sound( ) const
    {
        if ( not has_pawn(  ) )
        {
            return;
        }

        audio_ptr_->play( );
    }


    void TestController::volume_up( ) const
    {
        if ( not has_pawn(  ) )
        {
            return;
        }

        auto& ss = engine::SERVICE_LOCATOR.get_sound_system( );
        const float volume{ ss.get_master_volume( ) };
        ss.set_master_volume( volume + 0.01f );
    }


    void TestController::volume_down( ) const
    {
        if ( not has_pawn(  ) )
        {
            return;
        }

        auto& ss = engine::SERVICE_LOCATOR.get_sound_system( );
        const float volume{ ss.get_master_volume( ) };
        ss.set_master_volume( volume - 0.01f );
    }


    void TestController::kill( )
    {
        if ( not has_pawn(  ) )
        {
            return;
        }

        get_pawn( )->mark_for_deletion( );
    }


    void TestController::quit( ) const
    {
        SDL_Event quitEvent{ .type = SDL_QUIT };
        SDL_PushEvent( &quitEvent );
    }

}
