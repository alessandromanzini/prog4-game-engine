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
#include <singleton/GameInstance.h>
#include <singleton/GameTime.h>
#include <singleton/Renderer.h>
#include <state/character/AttackState.h>
#include <state/character/character_conditions.h>
#include <state/character/JumpState.h>
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
                                            std::unique_ptr<MoveCommand>&& moveCommand,
                                            const bool attackOnEnter )
        : Component( owner )
        , resources_{ resources }
        , attack_command_ptr_{ std::move( attackCommand ) }
        , jump_command_ptr_{ std::move( jumpCommand ) }
        , move_command_ptr_{ std::move( moveCommand ) }
        , attack_on_enter_{ attackOnEnter }
    {
        if ( const auto collider = get_owner( ).get_component<BoxColliderComponent>( ) )
        {
            size_ = collider.value( ).get_pivots( )[2];
        }
        fill_running_sum( );
        initialize_resources( );
        create_transitions( );
    }


    void CharacterComponent::tick( )
    {
        vertical_position_wrap( );
        average_positions( );
        manage_iframes( );
        state_machine_.tick( );
    }


    void CharacterComponent::render( ) const
    {
        Sprite2D* currentSprite{ nullptr };
        blackboard_.retrieve( UID( "current_sprite" ), currentSprite );
        if ( currentSprite && not is_iframe_swap_ )
        {
            currentSprite->flip( move_command_ptr_->is_facing_right( ), false );
            RENDERER.set_z_index( 2 );
            currentSprite->render( get_owner( ).get_world_transform( ).get_position( ) );
            RENDERER.set_z_index( 0 );
        }
    }


    void CharacterComponent::set_start_position( const glm::vec2 position )
    {
        blackboard_.edit( UID( "reposition_target" ), position );
    }


    glm::vec2 CharacterComponent::get_relative_movement( ) const
    {
        const glm::vec2 average = running_sum_ / static_cast<float>( prev_positions_.size( ) );
        return get_owner( ).get_world_transform( ).get_position( ) - average;
    }


    void CharacterComponent::move( glm::vec2 movement ) const
    {
        if ( const auto state = static_cast<CharacterState*>( state_machine_.get_current_state( ) ); state->can_move( ) )
        {
            movement.y = 0.f;
            move_command_ptr_->set_movement( movement );
            move_command_ptr_->execute( );
        }
    }


    void CharacterComponent::jump( )
    {
        if ( const auto state = static_cast<CharacterState*>( state_machine_.get_current_state( ) );
            state->can_jump( ) && jump_command_ptr_->can_jump( ) )
        {
            state_machine_.force_transition( UID( "jump" ) );
        }
    }


    void CharacterComponent::attack( )
    {
        if ( const auto state = static_cast<CharacterState*>( state_machine_.get_current_state( ) ); state->can_attack( ) )
        {
            attack_command_ptr_->set_attack_direction( move_command_ptr_->is_facing_right( )
                                                           ? glm::vec2{ 1.f, 0.f }
                                                           : glm::vec2{ -1.f, 0.f } );
            state_machine_.force_transition( UID( "attack" ) );
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


    bool CharacterComponent::is_repositioning( ) const
    {
        return state_machine_.get_current_state_id( ) == UID( "reposition" );
    }


    bool CharacterComponent::is_falling( ) const
    {
        return state_machine_.get_current_state_id( ) == UID( "fall" );
    }


    std::pair<bool, float> CharacterComponent::is_iframing( ) const
    {
        float iframes{ 0.f };
        blackboard_.retrieve( UID( "iframes" ), iframes );
        return { iframes > 0.f || is_repositioning(  ), iframes };
    }


    void CharacterComponent::fill_running_sum( )
    {
        const glm::vec2 position = get_owner( ).get_world_transform( ).get_position( );
        for ( auto& prevPosition : prev_positions_ )
        {
            running_sum_ += position;
            prevPosition = position;
        }
    }


    void CharacterComponent::initialize_resources( )
    {
        AudioComponent& attackAudio = get_owner( ).add_component<AudioComponent>( resources_.attack_audio_path,
                                                                                  sound::SoundType::SOUND_EFFECT,
                                                                                  UID( AudioCue::SFX ) );
        AudioComponent& bounceAudio = get_owner( ).add_component<AudioComponent>( resources_.bounce_audio_path,
                                                                                  sound::SoundType::SOUND_EFFECT,
                                                                                  UID( AudioCue::SFX ) );
        bounceAudio.set_volume( .05f );

        AudioComponent& jumpAudio = get_owner( ).add_component<AudioComponent>( resources_.jump_audio_path,
                                                                                sound::SoundType::SOUND_EFFECT,
                                                                                UID( AudioCue::SFX ) );
        jumpAudio.set_volume( .7f );

        // UTILITIES
        blackboard_.store( UID( "object" ), &get_owner( ) );
        blackboard_.store( UID( "relative_movement" ), glm::vec2{} );
        blackboard_.store( UID( "reposition_target" ), glm::vec2{} );
        blackboard_.store( UID( "repositioning" ), false );
        blackboard_.store( UID( "iframes" ), 0.f );
        blackboard_.store<Sprite2D*>( UID( "current_sprite" ), nullptr );

        // CREATE STATES
        state_machine_.create_state<fsm::State>( UID( "grounded" ) );
        state_machine_.create_state<fsm::State>( UID( "airborne" ) );
        state_machine_.mark_intermediate_state( UID( "grounded" ) );
        state_machine_.mark_intermediate_state( UID( "airborne" ) );


        state_machine_.create_state<CharacterState>( UID( "idle" ), &resources_.idle_sprite, nullptr, true, true, true );

        state_machine_.create_state<CharacterState>( UID( "walk" ), &resources_.walk_sprite, nullptr, true, true, true );

        state_machine_.create_state<CharacterState>( UID( "rise" ), &resources_.rise_sprite, &bounceAudio, true, false, true );

        state_machine_.create_state<CharacterState>( UID( "fall" ), &resources_.fall_sprite, nullptr, true, false, true );

        state_machine_.create_state<JumpState>( UID( "jump" ), &jumpAudio, jump_command_ptr_.get( ) );
        state_machine_.mark_intermediate_state( UID( "jump" ) );

        state_machine_.create_state<AttackState>( UID( "attack" ), &resources_.attack_sprite, &attackAudio,
                                                  attack_command_ptr_.get( ), attack_on_enter_ );

        state_machine_.create_state<RepositionState>( UID( "reposition" ), &resources_.reposition_sprite, nullptr );
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
        state_machine_.add_transition<cnd::TrueCondition>( UID( "jump" ), UID( "rise" ) );

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


    void CharacterComponent::vertical_position_wrap( ) const
    {
        constexpr float WRAP_EPSILON{ 1.f };
        glm::vec2 relativeMovement{};
        blackboard_.retrieve( UID( "relative_movement" ), relativeMovement );

        if ( glm::vec2 position = get_owner( ).get_world_transform( ).get_position( );
            position.y < -size_.y && relativeMovement.y < -WRAP_EPSILON )
        {
            position.y = GAME_INSTANCE.get_screen_dimensions( ).y;
            get_owner( ).set_world_transform( Transform::from_translation( position ) );
        }
        else if ( position.y > GAME_INSTANCE.get_screen_dimensions( ).y && relativeMovement.y > WRAP_EPSILON )
        {
            position.y = -size_.y;
            get_owner( ).set_world_transform( Transform::from_translation( position ) );
        }
    }


    void CharacterComponent::average_positions( )
    {
        const glm::vec2 currentPosition = get_owner( ).get_world_transform( ).get_position( );

        running_sum_ -= prev_positions_[current_position_index_];
        running_sum_ += currentPosition;

        prev_positions_[current_position_index_] = currentPosition;
        current_position_index_                  = ( current_position_index_ + 1 ) % prev_positions_.size( );

        blackboard_.edit( UID( "relative_movement" ), get_relative_movement( ) );
    }


    void CharacterComponent::manage_iframes( )
    {
        auto [iframing, iframes] = is_iframing( );

        iframes -= GAME_TIME.get_delta_time( );
        if ( not iframing )
        {
            blackboard_.edit( UID( "iframes" ), 0.f );
            iframe_swap_accumulated_ = 0.f;
            is_iframe_swap_ = false;
        }
        else
        {
            iframe_swap_accumulated_ += GAME_TIME.get_delta_time( );
            if ( iframe_swap_accumulated_ > IFRAMES_SWAP_ )
            {
                is_iframe_swap_ = not is_iframe_swap_;
                iframe_swap_accumulated_ -= IFRAMES_SWAP_;
            }
            blackboard_.edit( UID( "iframes" ), iframes );
        }
    }

}
