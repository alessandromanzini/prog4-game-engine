#include "CharacterComponent.h"

#include <command/AttackCommand.h>
#include <command/JumpCommand.h>
#include <command/MoveCommand.h>

#include <framework/GameObject.h>
#include <framework/component/physics/PhysicsComponent.h>
#include <framework/resource/texture/Sprite2D.h>

#include <framework/behaviour/fsm/transitions.h>
#include <singleton/Renderer.h>
#include <state/character/character_conditions.h>

namespace cnd = game::condition;
namespace lgc = engine::fsm::logic;

using namespace engine;
using namespace game::state;


namespace game
{
    CharacterComponent::CharacterComponent( owner_t& owner, GameObject& projectileSocket )
        : Component( owner )
        , state_machine_{ blackboard_ }

        , attack_command_{ owner, projectileSocket, PROJECTILE_SPEED_, PROJECTILE_OFFSET_ }
        , jump_command_{ owner, JUMP_FORCE_ }
        , move_command_{ owner, MOVEMENT_SPEED_ }

        , idle_sprite_{
            "characters/bub/bub_idle_1x8.png",
            static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 8u ),
            0.2f, 2.f
        }
        , walk_sprite_{
            "characters/bub/bub_walk_1x6.png",
            static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 6u ),
            0.2f, 2.f,
            { 0.f, -4.f }
        }
        , rise_sprite_{
            "characters/bub/bub_jump_1x2.png",
            static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 2u ),
            0.2f, 2.f
        }
        , fall_sprite_{
            "characters/bub/bub_fall_1x1.png",
            static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 1u ),
            0.2f, 2.f
        }
        , attack_sprite_{
            "characters/bub/bub_attack_1x3.png",
            static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 3u ),
            .1f, 2.f, {}, false
        }
    {
        // UTILITIES
        blackboard_.store( UID( "relative_movement" ), glm::vec2{ 0.f, 0.f } );
        blackboard_.store<Sprite2D*>( UID( "current_sprite" ), nullptr );

        // CREATE STATES
        state_machine_.create_state<fsm::State>( UID( "grounded" ) );
        state_machine_.create_state<fsm::State>( UID( "airborne" ) );
        state_machine_.mark_intermediate_state( UID( "grounded" ) );
        state_machine_.mark_intermediate_state( UID( "airborne" ) );

        state_machine_.create_state<CharacterState>( UID( "idle" ), &idle_sprite_, true, true, true );
        state_machine_.create_state<CharacterState>( UID( "walk" ), &walk_sprite_, true, true, true );
        state_machine_.create_state<CharacterState>( UID( "rise" ), &rise_sprite_, true, false, true );
        state_machine_.create_state<CharacterState>( UID( "fall" ), &fall_sprite_, true, false, true );
        state_machine_.create_state<CharacterState>( UID( "attack" ), &attack_sprite_, true, false, false );

        // IDLE
        state_machine_.add_transition<cnd::IsMovingHorzCondition>( UID( "idle" ), UID( "walk" ) );
        state_machine_.add_transition<lgc::Not<cnd::IsGroundedCondition>>( UID( "idle" ), UID( "airborne" ) );

        // WALK
        state_machine_.add_transition<lgc::Not<cnd::IsMovingHorzCondition>>( UID( "walk" ), UID( "idle" ) );
        state_machine_.add_transition<lgc::Not<cnd::IsGroundedCondition>>( UID( "walk" ), UID( "airborne" ) );

        // GROUNDED
        state_machine_.add_transition<lgc::Not<cnd::IsMovingHorzCondition>>( UID( "grounded" ), UID( "idle" ) );
        state_machine_.add_transition<cnd::IsMovingHorzCondition>( UID( "grounded" ), UID( "walk" ) );

        // JUMP
        state_machine_.add_transition<lgc::Not<cnd::IsRisingCondition>>( UID( "rise" ), UID( "fall" ) );
        state_machine_.add_transition<cnd::IsGroundedCondition>( UID( "rise" ), UID( "grounded" ) );

        // FALL
        state_machine_.add_transition<cnd::IsRisingCondition>( UID( "fall" ), UID( "rise" ) );
        state_machine_.add_transition<cnd::IsGroundedCondition>( UID( "fall" ), UID( "grounded" ) );

        // AIRBORNE
        state_machine_.add_transition<cnd::IsRisingCondition>( UID( "airborne" ), UID( "rise" ) );
        state_machine_.add_transition<cnd::IsFallingCondition>( UID( "airborne" ), UID( "fall" ) );

        // ATTACK
        state_machine_.add_transition<lgc::And<cnd::IsAnimationCompletedCondition, cnd::IsGroundedCondition>>(
            UID( "attack" ), UID( "grounded" ) );
        state_machine_.add_transition<cnd::IsAnimationCompletedCondition>( UID( "attack" ), UID( "airborne" ) );

        // SET INITIAL STATE
        state_machine_.start( UID( "idle" ) );
    }


    void CharacterComponent::tick( )
    {
        average_positions( );
        state_machine_.tick( );
    }


    void CharacterComponent::render( ) const
    {
        Sprite2D* currentSprite{ nullptr };
        blackboard_.retrieve( UID( "current_sprite" ), currentSprite );
        if ( currentSprite )
        {
            currentSprite->flip( move_command_.is_facing_right( ), false );
            RENDERER.set_z_index( 2 );
            currentSprite->render( get_owner( ).get_world_transform( ).get_position( ) );
            RENDERER.set_z_index( 0 );
        }
    }


    void CharacterComponent::move( const float movement )
    {
        if ( const auto state = static_cast<CharacterState*>( state_machine_.get_current_state( ) ); state->can_move( ) )
        {
            move_command_.set_movement( movement );
            move_command_.execute( );
        }
    }


    void CharacterComponent::jump( )
    {
        if ( const auto state = static_cast<CharacterState*>( state_machine_.get_current_state( ) ); state->can_jump( ) )
        {
            jump_command_.execute( );
        }
    }


    void CharacterComponent::attack( )
    {
        if ( const auto state = static_cast<CharacterState*>( state_machine_.get_current_state( ) ); state->can_attack( ) )
        {
            state_machine_.force_transition( UID( "attack" ) );
            attack_command_.set_attack_direction( move_command_.is_facing_right( )
                                                      ? glm::vec2{ 1.f, 0.f }
                                                      : glm::vec2{ -1.f, 0.f } );
            attack_command_.execute( );
        }
    }


    void CharacterComponent::average_positions( )
    {
        const glm::vec2 currentPosition = get_owner( ).get_world_transform( ).get_position( );

        running_sum_ -= prev_positions_[current_position_index_];
        running_sum_ += currentPosition;

        prev_positions_[current_position_index_] = currentPosition;
        current_position_index_                  = ( current_position_index_ + 1 ) % prev_positions_.size( );

        const glm::vec2 average = running_sum_ / static_cast<float>( prev_positions_.size( ) );
        blackboard_.edit( UID( "relative_movement" ), currentPosition - average );
    }

}
