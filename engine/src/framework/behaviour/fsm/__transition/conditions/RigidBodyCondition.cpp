#include <framework/behaviour/fsm/__transition/condition/RigidBodyCondition.h>

#include <framework/GameObject.h>
#include <framework/component/physics/RigidBodyComponent.h>
#include <framework/resource/data/blackboard_ids.h>

#include <cassert>
#include <type_traits>


namespace engine::fsm::condition
{
    bool RigidBodyCondition::is_idle( const Blackboard& blackboard ) const
    {
        const auto velocity = get_rigid_body( blackboard ).get_velocity( );
        return glm::dot( velocity, velocity ) <= IDLE_THRESHOLD_ * IDLE_THRESHOLD_;
    }


    bool RigidBodyCondition::is_moving( const Blackboard& blackboard ) const
    {
        return not is_idle( blackboard );
    }


    bool RigidBodyCondition::is_falling( const Blackboard& blackboard ) const
    {
        return get_rigid_body( blackboard ).get_velocity( ).y > 0.f;
    }


    bool RigidBodyCondition::is_grounded( const Blackboard& blackboard ) const
    {
        constexpr float EPSILON = std::numeric_limits<float>::epsilon( );
        const float vely        = get_rigid_body( blackboard ).get_velocity( ).y;
        return vely > -EPSILON && vely < EPSILON;
    }


    const RigidBodyComponent& RigidBodyCondition::get_rigid_body( const Blackboard& blackboard ) const
    {
        RigidBodyComponent* rigidBody{ nullptr };
        blackboard.retrieve( ids::RIGID_BODY_UID, rigidBody );
        assert( rigidBody && "RigidBodyCondition::RigidBodyCondition: Missing rigid body" );
        return *rigidBody;
    }


    // +---------------------------+
    // | IS GROUNDED CONDITION     |
    // +---------------------------+
    bool IsGroundedCondition::evaluate( Blackboard& blackboard ) const
    {
        return is_grounded( blackboard );
    }


    // +---------------------------+
    // | IS IDLE CONDITION         |
    // +---------------------------+
    bool IsIdleCondition::evaluate( Blackboard& blackboard ) const
    {
        return is_idle( blackboard );
    }


    // +----------------------------------+
    // | IS MOVING HORIZONTALLY CONDITION |
    // +----------------------------------+
    bool IsMovingHorizontallyCondition::evaluate( Blackboard& blackboard ) const
    {
        constexpr float EPSILON = std::numeric_limits<float>::epsilon( );
        const float velx        = get_rigid_body( blackboard ).get_velocity( ).x;
        return velx < -EPSILON || velx > EPSILON;
    }


    // +----------------------------------+
    // | IS FALLING CONDITION             |
    // +----------------------------------+
    bool IsFallingCondition::evaluate( Blackboard& blackboard ) const
    {
        return is_falling( blackboard );
    }


}
