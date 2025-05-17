#include <framework/GameObject.h>
#include <framework/component/physics/RigidBodyComponent.h>

#include <framework/Transform.h>
#include <singleton/GameInstance.h>
#include <singleton/GameTime.h>


namespace engine
{
    RigidBodyComponent::RigidBodyComponent( owner_t& owner )
        : Component{ owner } { }


    void RigidBodyComponent::set_simulate_physics( const bool simulate )
    {
        simulate_physics_ = simulate;
    }


    bool RigidBodyComponent::is_simulating_physics( ) const
    {
        return simulate_physics_;
    }


    glm::vec2 RigidBodyComponent::get_velocity( ) const
    {
        return velocity_;
    }


    void RigidBodyComponent::add_force( const glm::vec2 force, const bool acceleration )
    {
        if ( acceleration )
        {
            velocity_ += force * GAME_TIME.get_delta_time( );
        }
        else
        {
            velocity_ += force;
        }
    }


    void RigidBodyComponent::tick( )
    {
        const glm::vec2 g{ 0.f, GAME_INSTANCE.get_gravity_coefficient( ) };
        const auto displacement{
            simulate_physics_ ? ( velocity_ + g ) * GAME_TIME.get_delta_time( ) : velocity_ * GAME_TIME.get_delta_time( )
        };

        get_owner( ).get_transform_operator( ).translate( displacement );
    }

}
