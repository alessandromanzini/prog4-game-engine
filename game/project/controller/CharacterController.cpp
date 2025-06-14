#include "CharacterController.h"

#include <component/CharacterComponent.h>
#include <component/JoinMenuComponent.h>
#include <registration/input.h>

#include <cassert>
#include <component/GameOverComponent.h>


namespace game
{
    CharacterController::CharacterController( JoinMenuComponent* joinComponent, engine::binding::DeviceType deviceType )
        : join_menu_component_ptr_{ joinComponent }
    {
        try_register_device( deviceType );
    }


    void CharacterController::possess( engine::GameObject* pawn )
    {
        PlayerController::possess( pawn );

        state_ptr_ = &pawn->get_component<CharacterComponent>( ).value( );
        assert( state_ptr_ && "TestController::possess: Missing components" );
    }


    bool CharacterController::has_joined( ) const
    {
        return has_joined_;
    }


    void CharacterController::set_block_selection( const bool block )
    {
        block_selection_ = block;
    }


    void CharacterController::set_gameover_component( GameOverComponent* gameover )
    {
        game_over_component_ptr_ = gameover;
    }


    void CharacterController::device_registered( engine::InputMappingContext& context,
                                                 const engine::binding::DeviceInfo deviceInfo )
    {
        device_info_ = deviceInfo;
        printf( "CHARACTER CONTROLLER: registered device at id %d\n", deviceInfo.id );

        context.bind_to_input_action( this, engine::UID( IA::MOVE ), &CharacterController::move,
                                      engine::binding::TriggerEvent::PRESSED );
        context.bind_to_input_action( this, engine::UID( IA::MOVE ), &CharacterController::move,
                                      engine::binding::TriggerEvent::RELEASED );

        context.bind_to_input_action( this, engine::UID( IA::JUMP ), &CharacterController::jump,
                                      engine::binding::TriggerEvent::TRIGGERED );

        context.bind_to_input_action( this, engine::UID( IA::ATTACK ), &CharacterController::attack,
                                      engine::binding::TriggerEvent::TRIGGERED );

        context.bind_to_input_action( this, engine::UID( IA::JOIN ), &CharacterController::join );
        context.bind_to_input_action( this, engine::UID( IA::LEAVE ), &CharacterController::leave );
        context.bind_to_input_action( this, engine::UID( IA::MODE_SELECTION ), &CharacterController::menu_select );
        context.bind_to_input_action( this, engine::UID( IA::CONFIRM ), &CharacterController::confirm );
        context.bind_to_input_action( this, engine::UID( IA::NAME_SELECTION ), &CharacterController::name_select );
    }


    void CharacterController::move( const float dir ) const
    {
        if ( not has_pawn( ) )
        {
            return;
        }
        state_ptr_->move( { dir, 0.f } );
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


    void CharacterController::join( )
    {
        if ( block_selection_ ) { return; }
        has_joined_ = join_menu_component_ptr_->join( device_info_ );
    }


    void CharacterController::leave( )
    {
        if ( block_selection_ ) { return; }
        join_menu_component_ptr_->leave( device_info_.id );
        has_joined_ = false;
    }


    void CharacterController::menu_select( const float direction ) const
    {
        if ( not has_joined_ || block_selection_ )
        {
            return;
        }

        if ( direction < 0.0f )
        {
            join_menu_component_ptr_->prev_selection( );
        }
        else
        {
            join_menu_component_ptr_->next_selection( );
        }
    }


    void CharacterController::confirm( ) const
    {
        if ( has_joined_ && game_over_component_ptr_ )
        {
            game_over_component_ptr_->confirm( );
            return;
        }
        if ( not has_joined_ || block_selection_ )
        {
            return;
        }
        join_menu_component_ptr_->confirm_selection( );
    }


    void CharacterController::name_select( const glm::vec2 selection ) const
    {
        if ( not has_joined_ || not game_over_component_ptr_ )
        {
            return;
        }
        if ( selection.x == 1.f )
        {
            game_over_component_ptr_->letter_horz( false );
        }
        if ( selection.x == -1.f )
        {
            game_over_component_ptr_->letter_horz( true );
        }
        if ( selection.y == 1.f )
        {
            game_over_component_ptr_->letter_vert( true );
        }
        if ( selection.y == -1.f )
        {
            game_over_component_ptr_->letter_vert( false );
        }
    }

}
