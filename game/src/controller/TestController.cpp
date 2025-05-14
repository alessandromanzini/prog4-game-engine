#include <controller/TestController.h>

#include <framework/component/AudioComponent.h>
#include <framework/component/RigidBodyComponent.h>
#include <registration/input.h>

#include <iostream>

#include <SDL.h>
#include <singleton/ServiceLocator.h>


namespace game
{
    TestController::TestController( )
    {
        try_register_device( engine::binding::DeviceType::GAMEPAD );
    }


    void TestController::possess( engine::GameObject* pawn )
    {
        PlayerController::possess( pawn );

        audio_ptr_ = &pawn->get_component<engine::AudioComponent>( );
        rigid_body_ptr_  = &pawn->get_component<engine::RigidBodyComponent>( );

        assert( audio_ptr_ && rigid_body_ptr_ && "TestController::possess: Missing components" );
    }


    void TestController::device_registered( engine::InputMappingContext& context, const engine::binding::DeviceInfo deviceInfo )
    {
        std::cout << "Test device registered at id " << static_cast<int>( deviceInfo.id ) << '\n';

        context.bind_to_input_action( *this, engine::UID( IA::MOVE ), &TestController::move,
                                      engine::binding::TriggerEvent::PRESSED );
        context.bind_to_input_action( *this, engine::UID( IA::MOVE ), &TestController::move,
                                      engine::binding::TriggerEvent::RELEASED );

        context.bind_to_input_action( *this, engine::UID( IA::JUMP ), &TestController::jump,
                                      engine::binding::TriggerEvent::TRIGGERED );

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

        rigid_body_ptr_->reset_horizontal_velocity( );
        if ( dir.x != 0.f )
        {
            rigid_body_ptr_->add_velocity( { dir.x * 100.f, 0.f } );
        }
    }


    void TestController::jump( ) const
    {
        if ( not has_pawn(  ) )
        {
            return;
        }

        if ( rigid_body_ptr_->get_velocity( ).y == 0.f )
        {
            rigid_body_ptr_->add_velocity( { 0.f, -100.f } );
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
