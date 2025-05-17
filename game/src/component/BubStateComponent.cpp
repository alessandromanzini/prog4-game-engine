#include <component/BubStateComponent.h>

#include <framework/GameObject.h>
#include <framework/component/physics/RigidBodyComponent.h>

#include <framework/behaviour/fsm/transitions.h>
#include <framework/resource/data/blackboard_ids.h>

#include <state/SpriteState.h>

namespace cnd = engine::fsm::condition;
namespace lgc = engine::fsm::logic;

using namespace engine;
using namespace game::state;


namespace game
{
    BubStateComponent::BubStateComponent( owner_t& owner )
        : Component( owner )
        , state_machine_{ blackboard_ }
    {
        blackboard_.store( ids::RIGID_BODY_UID, &get_owner( ).get_component<RigidBodyComponent>( ) );

        // TODO: fix composition to point to base state
        sprites_.insert( {
            UID( CharacterState::IDLE ),
            Sprite2D{
                "characters/bub/bub_idle_1x8.png",
                static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 8u ), 0.2f, 2.f
            }
        } );

        sprites_.insert( {
            UID( CharacterState::WALK ),
            Sprite2D{
                "characters/bub/bub_walk_1x6.png",
                static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 6u ),
                0.2f, 2.f, { 0.f, -3.f }
            }
        } );

        sprites_.insert( {
            UID( CharacterState::JUMP ),
            Sprite2D{
                "characters/bub/bub_jump_1x2.png",
                static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 2u ),
                0.2f, 2.f, {}, false
            }
        } );

        sprites_.insert( {
            UID( CharacterState::FALL ),
            Sprite2D{
                "characters/bub/bub_fall_1x1.png",
                static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 1u ),
                0.2f, 2.f
            }
        } );


        add_sprite_state( UID( CharacterState::IDLE ) );
        add_sprite_state( UID( CharacterState::WALK ) );
        add_sprite_state( UID( CharacterState::JUMP ) );
        add_sprite_state( UID( CharacterState::FALL ) );

        state_machine_.add_transition<cnd::IsMovingHorizontallyCondition>(
            UID( CharacterState::IDLE ), UID( CharacterState::WALK ) );
        state_machine_.add_transition<cnd::IsIdleCondition>( UID( CharacterState::WALK ), UID( CharacterState::IDLE ) );

        state_machine_.add_transition<cnd::Negate<cnd::IsGroundedCondition>>(
            UID( CharacterState::WALK ), UID( CharacterState::JUMP ) );
        state_machine_.add_transition<cnd::Negate<cnd::IsGroundedCondition>>(
            UID( CharacterState::IDLE ), UID( CharacterState::JUMP ) );

        state_machine_.add_transition<cnd::IsFallingCondition>( UID( CharacterState::JUMP ), UID( CharacterState::FALL ) );

        state_machine_.add_transition<cnd::Combine<lgc::And, cnd::IsGroundedCondition, cnd::IsIdleCondition>>(
            UID( CharacterState::FALL ), UID( CharacterState::IDLE ) );
        state_machine_.add_transition<cnd::Combine<lgc::And, cnd::IsGroundedCondition, cnd::Negate<cnd::IsIdleCondition>>>(
            UID( CharacterState::FALL ), UID( CharacterState::WALK ) );

        state_machine_.start( UID( CharacterState::IDLE ) );
    }


    void BubStateComponent::tick( )
    {
        state_machine_.tick( );

        if ( not sprite_ptr_ ) { return; }
        sprite_ptr_->tick( );
    }


    void BubStateComponent::render( ) const
    {
        if ( not sprite_ptr_ ) { return; }
        sprite_ptr_->render( get_owner( ).get_world_transform( ).get_position( ) );
    }


    void BubStateComponent::notify( const UID event, Subject*, event::broadcast_value_variant_t )
    {
        if ( event == NULL_UID )
        {
            sprite_ptr_ = nullptr;
        }
        else
        {
            sprite_ptr_ = &sprites_.at( event );
        }
    }


    void BubStateComponent::add_sprite_state( UID uid )
    {
        assert( sprites_.contains( uid ) && "State not found in sprites map!" );
        auto& state = state_machine_.create_state<SpriteState>( uid, &sprites_.at( uid ), uid );
        state.add_observer( *this );
    }

}
