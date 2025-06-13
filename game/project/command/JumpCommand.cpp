#include "JumpCommand.h"

#include <framework/GameObject.h>
#include <framework/component/physics/PhysicsComponent.h>

using namespace std::chrono;

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
        if ( can_jump( ) )
        {
            physics_body_ptr_->add_force( { 0.f, -force_ } );
            jump_time_ = system_clock::now( );
        }
    }


    bool JumpCommand::can_jump( ) const
    {
        return duration_cast<seconds>( system_clock::now( ) - jump_time_ ).count( ) > JUMP_TIME_DELAY_;
    }

}
