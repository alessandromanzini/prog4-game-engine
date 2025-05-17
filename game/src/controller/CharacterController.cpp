#include <controller/CharacterController.h>

#include <framework/component/physics/RigidBodyComponent.h>
#include <registration/input.h>

#include <cassert>


namespace game
{
    CharacterController::CharacterController()
    {
        try_register_device( engine::binding::DeviceType::GAMEPAD );
    }


    void CharacterController::possess( engine::GameObject* pawn )
    {
        PlayerController::possess( pawn );

        rigid_body_ptr_  = &pawn->get_component<engine::RigidBodyComponent>( );
        assert( rigid_body_ptr_ && "TestController::possess: Missing components" );
    }


    void CharacterController::device_registered( engine::InputMappingContext& context, const engine::binding::DeviceInfo deviceInfo )
    {
        printf( "CHARACTER CONTROLLER: registered device at id %d\n", deviceInfo.id );

        context.bind_to_input_action( this, engine::UID( IA::MOVE ), &CharacterController::move,
                                      engine::binding::TriggerEvent::PRESSED );
        // context.bind_to_input_action( this, engine::UID( IA::MOVE ), &CharacterController::move,
        //                               engine::binding::TriggerEvent::RELEASED );

        context.bind_to_input_action( this, engine::UID( IA::JUMP ), &CharacterController::jump,
                                      engine::binding::TriggerEvent::TRIGGERED );

        context.bind_to_input_action( this, engine::UID( IA::KILL ), &CharacterController::kill,
                                      engine::binding::TriggerEvent::TRIGGERED );
    }


    void CharacterController::move( const glm::vec2 dir ) const
    {
        if ( not has_pawn(  ) )
        {
            return;
        }

        rigid_body_ptr_->add_force( { dir.x * 100.f, 0.f } );
    }


    void CharacterController::jump( ) const
    {
        if ( not has_pawn(  ) )
        {
            return;
        }

        if ( rigid_body_ptr_->get_velocity( ).y == 0.f )
        {
            rigid_body_ptr_->add_force( { 0.f, -100.f } );
        }

    }


    void CharacterController::kill( )
    {
        if ( not has_pawn(  ) )
        {
            return;
        }

        get_pawn( )->mark_for_deletion( );
    }

}
