#include "RepositionState.h"

#include <component/CharacterComponent.h>
#include <component/CollisionsComponent.h>
#include <framework/GameObject.h>
#include <framework/component/physics/BoxColliderComponent.h>
#include <singleton/GameInstance.h>
#include <singleton/GameTime.h>


namespace game
{
    RepositionState::RepositionState( engine::Sprite2D* sprite, engine::AudioComponent* audio )
        : CharacterState( sprite, audio, false, false, false ) { }


    void RepositionState::on_enter( engine::Blackboard& blackboard )
    {
        CharacterState::on_enter( blackboard );

        blackboard.edit( engine::UID( "repositioning" ), true );

        engine::GameObject* object{ nullptr };
        blackboard.retrieve( engine::UID( "object" ), object );
        assert( object && "RepositionState::on_enter: Missing object in blackboard" );

        const auto character = object->get_component<CharacterComponent>( );
        character.value( ).set_physics_simulation( false );

        glm::vec2 target{};
        blackboard.retrieve( engine::UID( "reposition_target" ), target );
        direction_ = ( target - object->get_world_transform(  ).get_position(  ) ) / REPOSITION_TIME_;
        accumulator_ = 0.f;
    }


    void RepositionState::on_exit( engine::Blackboard& blackboard )
    {
        CharacterState::on_exit( blackboard );

        engine::GameObject* object{ nullptr };
        blackboard.retrieve( engine::UID( "object" ), object );
        assert( object && "RepositionState::on_enter: Missing object in blackboard" );

        const auto character = object->get_component<CharacterComponent>( );
        character.value( ).set_physics_simulation( true );

        blackboard.edit( engine::UID( "iframes" ), REPOSITION_IFRAMES_ );
    }


    void RepositionState::tick( engine::Blackboard& blackboard )
    {
        CharacterState::tick( blackboard );

        engine::GameObject* object{ nullptr };
        blackboard.retrieve( engine::UID( "object" ), object );
        assert( object && "RepositionState::on_enter: Missing object in blackboard" );

        object->get_transform_operator( ).translate( direction_ * engine::GAME_TIME.get_delta_time( ) );
        accumulator_ += engine::GAME_TIME.get_delta_time( );
        if ( accumulator_ >= REPOSITION_TIME_ )
        {
            blackboard.edit( engine::UID( "repositioning" ), false );
        }
    }

}
