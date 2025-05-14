#include <framework/behaviour/fsm/__transitions/conditions/RigidBodyCondition.h>

#include <framework/GameObject.h>
#include <framework/components/RigidBodyComponent.h>
#include <framework/resources/data/blackboard_ids.h>

#include <type_traits>
#include <cassert>


namespace engine::fsm::conditions
{
        RigidBodyCondition::RigidBodyCondition( Blackboard& blackboard )
        {
            blackboard.retrieve( ids::RIGID_BODY_UID, rigid_body_ptr_ );
            assert( rigid_body_ptr_ && "RigidBodyCondition::RigidBodyCondition: Missing rigid body" );
        }


        bool RigidBodyCondition::is_idle( ) const
        {
            const auto velocity = rigid_body_ptr_->get_velocity( );
            return glm::dot( velocity, velocity ) <= IDLE_THRESHOLD_ * IDLE_THRESHOLD_;
        }


        bool RigidBodyCondition::is_moving( ) const
        {
            return not is_idle( );
        }


        bool RigidBodyCondition::is_falling( ) const
        {
            return get_rigid_body( ).get_velocity( ).y > 0.f;
        }


        bool RigidBodyCondition::is_grounded( ) const
        {
            constexpr float EPSILON = std::numeric_limits<float>::epsilon( );
            const float vely        = get_rigid_body( ).get_velocity( ).y;
            return vely > -EPSILON && vely < EPSILON;
        }


        const RigidBodyComponent& RigidBodyCondition::get_rigid_body( ) const
        {
            return *rigid_body_ptr_;
        }


        // +---------------------------+
        // | IS GROUNDED CONDITION     |
        // +---------------------------+
        IsGroundedCondition::IsGroundedCondition( Blackboard& blackboard )
            : RigidBodyCondition( blackboard ) { }


        bool IsGroundedCondition::evaluate( Blackboard& ) const
        {
            return is_grounded( );
        }


        // +---------------------------+
        // | IS IDLE CONDITION         |
        // +---------------------------+
        IsIdleCondition::IsIdleCondition( Blackboard& blackboard )
            : RigidBodyCondition( blackboard ) { }


        bool IsIdleCondition::evaluate( Blackboard& ) const
        {
            return is_idle( );
        }


        // +----------------------------------+
        // | IS MOVING HORIZONTALLY CONDITION |
        // +----------------------------------+
        IsMovingHorizontallyCondition::IsMovingHorizontallyCondition( Blackboard& blackboard )
            : RigidBodyCondition( blackboard ) { }


        bool IsMovingHorizontallyCondition::evaluate( Blackboard& ) const
        {
            constexpr float EPSILON = std::numeric_limits<float>::epsilon( );
            const float velx        = get_rigid_body( ).get_velocity( ).x;
            return velx < -EPSILON || velx > EPSILON;
        }


}