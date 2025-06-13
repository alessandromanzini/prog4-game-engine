#include "CharacterComponent.h"

#include <command/AttackCommand.h>
#include <command/JumpCommand.h>
#include <command/MoveCommand.h>

#include <framework/GameObject.h>
#include <framework/component/physics/PhysicsComponent.h>
#include <framework/resource/texture/Sprite2D.h>

#include <framework/behaviour/fsm/transitions.h>
#include <framework/component/AudioComponent.h>
#include <framework/component/physics/BoxColliderComponent.h>
#include <registration/audio.h>
#include <singleton/Renderer.h>
#include <state/character/character_conditions.h>
#include <state/character/RepositionState.h>

namespace cnd = game::condition;
namespace lgc = engine::fsm::logic;

using namespace engine;
using namespace game::state;


namespace game
{
    CharacterComponent::CharacterComponent( owner_t& owner, const CharacterResources& resources,
                                            std::unique_ptr<AttackCommand>&& attackCommand,
                                            std::unique_ptr<JumpCommand>&& jumpCommand,
                                            std::unique_ptr<MoveCommand>&& moveCommand )
        : Component( owner )
        , resources_{ resources }
        , attack_command_ptr_{ std::move( attackCommand ) }
        , jump_command_ptr_{ std::move( jumpCommand ) }
        , move_command_ptr_{ std::move( moveCommand ) }
    {
        initialize_resources( );
        create_transitions( );
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
            currentSprite->flip( move_command_ptr_->is_facing_right( ), false );
            RENDERER.set_z_index( 2 );
            currentSprite->render( get_owner( ).get_world_transform( ).get_position( ) );
            RENDERER.set_z_index( 0 );
        }
    }


    void CharacterComponent::move( const glm::vec2 movement ) const
    {
        if ( const auto state = static_cast<CharacterState*>( state_machine_.get_current_state( ) ); state->can_move( ) )
        {
            move_command_ptr_->set_movement( movement );
            move_command_ptr_->execute( );
        }
    }


    void CharacterComponent::jump( ) const
    {
        if ( const auto state = static_cast<CharacterState*>( state_machine_.get_current_state( ) ); state->can_jump( ) )
        {
            jump_command_ptr_->execute( );
            if ( jump_audio_ptr_ )
            {
                jump_audio_ptr_->play( );
            }
        }
    }


    void CharacterComponent::attack( )
    {
        if ( const auto state = static_cast<CharacterState*>( state_machine_.get_current_state( ) ); state->can_attack( ) )
        {
            state_machine_.force_transition( UID( "attack" ) );
            attack_command_ptr_->set_attack_direction( move_command_ptr_->is_facing_right( )
                                                           ? glm::vec2{ 1.f, 0.f }
                                                           : glm::vec2{ -1.f, 0.f } );
            attack_command_ptr_->execute( );
            if ( attack_audio_ptr_ )
            {
                attack_audio_ptr_->play( );
            }
        }
    }


    void CharacterComponent::set_physics_simulation( const bool simulate ) const
    {
        get_owner( ).get_component<BoxColliderComponent>( ).value( ).set_enabled( simulate );
        auto& physics = get_owner( ).get_component<PhysicsComponent>( ).value( );
        physics.set_simulate_physics( simulate );
        physics.add_force( -physics.get_velocity( ) );
    }


    void CharacterComponent::reposition( )
    {
        state_machine_.force_transition( UID( "reposition" ) );
    }


    void CharacterComponent::initialize_resources( )
    {
        AudioComponent* bounceAudio{ nullptr };
        if ( not resources_.attack_audio_path.empty( ) )
        {
            attack_audio_ptr_ = &get_owner( ).add_component<AudioComponent>( resources_.attack_audio_path,
                                                                             sound::SoundType::SOUND_EFFECT,
                                                                             UID( AudioCue::SFX ) );
        }
        if ( not resources_.jump_audio_path.empty( ) )
        {
            jump_audio_ptr_ = &get_owner( ).add_component<AudioComponent>( resources_.jump_audio_path,
                                                                           sound::SoundType::SOUND_EFFECT,
                                                                           UID( AudioCue::SFX ) );
        }
        if ( not resources_.bounce_audio_path.empty( ) )
        {
            bounceAudio = &get_owner( ).add_component<AudioComponent>( resources_.bounce_audio_path,
                                                                       sound::SoundType::SOUND_EFFECT,
                                                                       UID( AudioCue::SFX ) );
            bounceAudio->set_volume( .05f );
        }

        // UTILITIES
        blackboard_.store( UID( "object" ), &get_owner( ) );
        blackboard_.store( UID( "relative_movement" ), glm::vec2{ 0.f, 0.f } );
        blackboard_.store( UID( "repositioning" ), false );
        blackboard_.store<Sprite2D*>( UID( "current_sprite" ), nullptr );

        // CREATE STATES
        state_machine_.create_state<fsm::State>( UID( "grounded" ) );
        state_machine_.create_state<fsm::State>( UID( "airborne" ) );
        state_machine_.mark_intermediate_state( UID( "grounded" ) );
        state_machine_.mark_intermediate_state( UID( "airborne" ) );


        state_machine_.create_state<CharacterState>( UID( "idle" ), &resources_.idle_sprite, nullptr, true, true, true );

        state_machine_.create_state<CharacterState>( UID( "walk" ), &resources_.walk_sprite, nullptr, true, true, true );

        state_machine_.create_state<CharacterState>( UID( "rise" ), &resources_.rise_sprite, bounceAudio, true, false, true );

        state_machine_.create_state<CharacterState>( UID( "fall" ), &resources_.fall_sprite, nullptr, true, false, true );

        state_machine_.create_state<CharacterState>( UID( "attack" ), &resources_.attack_sprite, nullptr, true, false, false );

        state_machine_.create_state<RepositionState>( UID( "reposition" ), &resources_.reposition_sprite, nullptr,
                                                      get_owner( ).get_world_transform( ).get_position( ) );
    }


    void CharacterComponent::create_transitions( )
    {
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

        // REPOSITION
        state_machine_.add_transition<lgc::And<lgc::Not<cnd::IsRepositioningCondition>, cnd::IsGroundedCondition>>(
            UID( "reposition" ), UID( "grounded" ) );
        state_machine_.add_transition<lgc::Not<cnd::IsRepositioningCondition>>(
            UID( "reposition" ), UID( "airborne" ) );

        // SET INITIAL STATE
        state_machine_.start( UID( "idle" ) );
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
