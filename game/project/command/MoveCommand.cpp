#include "MoveCommand.h"

#include <framework/GameObject.h>
#include <framework/component/physics/PhysicsComponent.h>


namespace game
{
    MoveCommand::MoveCommand( engine::GameObject& owner, const float speed )
        : GameObjectCommand( owner )
        , speed_{ speed }
    {
        physics_body_ptr_ = &get_object( ).get_component<engine::PhysicsComponent>( ).value( );
        assert( physics_body_ptr_ && "MoveCommand::MoveCommand: GameObject must own a PhysicsComponent." );
    }


    void MoveCommand::execute( )
    {
        physics_body_ptr_->add_force( { -physics_body_ptr_->get_velocity(  ).x, 0.f } );
        physics_body_ptr_->add_force( movement_ );
    }


    void MoveCommand::set_movement( const glm::vec2 movement )
    {
        movement_ = movement * speed_;
        if ( movement_.x != 0.f )
        {
            is_facing_right_ = movement_.x > 0.f;
        }
    }


    bool MoveCommand::is_facing_right( ) const
    {
        return is_facing_right_;
    }

}
