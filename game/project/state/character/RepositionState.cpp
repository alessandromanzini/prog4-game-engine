#include "RepositionState.h"

#include <component/CharacterComponent.h>
#include <component/CollisionsComponent.h>
#include <framework/GameObject.h>
#include <framework/component/physics/BoxColliderComponent.h>
#include <singleton/GameInstance.h>
#include <singleton/GameTime.h>


namespace game
{
    RepositionState::RepositionState( engine::Sprite2D* sprite, engine::AudioComponent* audio, const glm::vec2 target )
        : CharacterState( sprite, audio, false, false, false )
        , target_{ target } { }


    void RepositionState::on_enter( engine::Blackboard& blackboard )
    {
        CharacterState::on_enter( blackboard );

        blackboard.edit( engine::UID( "repositioning" ), true );

        engine::GameObject* object{ nullptr };
        blackboard.retrieve( engine::UID( "object" ), object );
        assert( object && "RepositionState::on_enter: Missing object in blackboard" );

        const auto character = object->get_component<game::CharacterComponent>( );
        character.value( ).set_physics_simulation( false );
    }


    void RepositionState::on_exit( engine::Blackboard& blackboard )
    {
        CharacterState::on_exit( blackboard );

        engine::GameObject* object{ nullptr };
        blackboard.retrieve( engine::UID( "object" ), object );
        assert( object && "RepositionState::on_enter: Missing object in blackboard" );

        const auto character = object->get_component<game::CharacterComponent>( );
        character.value( ).set_physics_simulation( true );
    }


    void RepositionState::tick( engine::Blackboard& blackboard )
    {
        CharacterState::tick( blackboard );

        engine::GameObject* object{ nullptr };
        blackboard.retrieve( engine::UID( "object" ), object );
        assert( object && "RepositionState::on_enter: Missing object in blackboard" );

        const auto difference = target_ - object->get_world_transform( ).get_position( );
        direction_ = normalize( difference );
        object->get_transform_operator( ).translate( direction_ * REPOSITION_SPEED * engine::GAME_TIME.get_delta_time( ) );
        printf("distance squared: %f, %f\n", direction_.x, direction_.y);
        if ( dot( difference, difference ) < 100.f )
        {
            blackboard.edit( engine::UID( "repositioning" ), false );
        }
    }

}
