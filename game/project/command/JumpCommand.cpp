#include "JumpCommand.h"

#include <framework/GameObject.h>
#include <framework/component/physics/PhysicsComponent.h>


namespace game
{
    JumpCommand::JumpCommand( engine::GameObject& owner, const float force )
        : GameObjectCommand( owner )
        , force_{ force }
    {
        physics_body_ptr_ = &get_object( ).get_component<engine::PhysicsComponent>( ).value( );
        assert( physics_body_ptr_ && "JumpCommand::JumpCommand: GameObject must own a PhysicsComponent." );
    }


    void JumpCommand::execute( )
    {
        physics_body_ptr_->add_force( { 0.f, -force_ } );
    }

}
