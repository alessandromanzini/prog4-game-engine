#include "LockState.h"

#include <component/CharacterComponent.h>
#include <framework/GameObject.h>


namespace game
{
    LockState::LockState( engine::Sprite2D* sprite, engine::AudioComponent* audio )
        : CharacterState{ sprite, audio, false, false, false } { }


    void LockState::on_enter( engine::Blackboard& blackboard )
    {
        CharacterState::on_enter( blackboard );

        blackboard.edit( engine::UID( "repositioning" ), true );

        engine::GameObject* object{ nullptr };
        blackboard.retrieve( engine::UID( "object" ), object );
        assert( object && "RepositionState::on_enter: Missing object in blackboard" );

        const auto character = object->get_component<CharacterComponent>( );
        character.value( ).set_physics_simulation( false );
    }


    void LockState::on_exit( engine::Blackboard& blackboard )
    {
        CharacterState::on_exit( blackboard );

        engine::GameObject* object{ nullptr };
        blackboard.retrieve( engine::UID( "object" ), object );
        assert( object && "RepositionState::on_enter: Missing object in blackboard" );

        const auto character = object->get_component<CharacterComponent>( );
        character.value( ).set_physics_simulation( true );
    }

}
