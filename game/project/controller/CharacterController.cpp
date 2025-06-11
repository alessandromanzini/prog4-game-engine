#include "CharacterController.h"

#include <component/CharacterComponent.h>
#include <registration/input.h>

#include <cassert>


namespace game
{
    CharacterController::CharacterController( )
    {
        try_register_device( engine::binding::DeviceType::KEYBOARD );
    }


    void CharacterController::possess( engine::GameObject* pawn )
    {
        PlayerController::possess( pawn );

        state_ptr_ = &pawn->get_component<CharacterComponent>( ).value( );
        assert( state_ptr_ && "TestController::possess: Missing components" );
    }


    void CharacterController::device_registered( engine::InputMappingContext& context,
                                                 const engine::binding::DeviceInfo deviceInfo )
    {
        printf( "CHARACTER CONTROLLER: registered device at id %d\n", deviceInfo.id );

        context.bind_to_input_action( this, engine::UID( IA::MOVE ), &CharacterController::move,
                                      engine::binding::TriggerEvent::PRESSED );
        context.bind_to_input_action( this, engine::UID( IA::MOVE ), &CharacterController::move,
                                      engine::binding::TriggerEvent::RELEASED );

        context.bind_to_input_action( this, engine::UID( IA::JUMP ), &CharacterController::jump,
                                      engine::binding::TriggerEvent::TRIGGERED );

        context.bind_to_input_action( this, engine::UID( IA::ATTACK ), &CharacterController::attack,
                                      engine::binding::TriggerEvent::TRIGGERED );
    }


    void CharacterController::move( const glm::vec2 dir ) const
    {
        if ( not has_pawn( ) )
        {
            return;
        }
        state_ptr_->move( dir );
    }


    void CharacterController::jump( ) const
    {
        if ( not has_pawn( ) )
        {
            return;
        }
        state_ptr_->jump( );
    }


    void CharacterController::attack( ) const
    {
        if ( not has_pawn( ) )
        {
            return;
        }
        state_ptr_->attack( );
    }

}
