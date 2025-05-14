#ifndef RIGIDBODYCONDITION_H
#define RIGIDBODYCONDITION_H

#include <framework/behaviour/fsm/__transition/base_transitions.h>


namespace engine
{
    class RigidBodyComponent;
}

namespace engine::fsm::conditions
{
    class RigidBodyCondition : public Condition
    {
    public:
        explicit RigidBodyCondition( Blackboard& blackboard );

        bool is_idle( ) const;
        bool is_moving( ) const;

        bool is_falling( ) const;
        bool is_grounded( ) const;

    protected:
        [[nodiscard]] const RigidBodyComponent& get_rigid_body( ) const;

    private:
        static constexpr float IDLE_THRESHOLD_ = 0.1f;

        const RigidBodyComponent* rigid_body_ptr_;

    };

    class IsGroundedCondition final : public RigidBodyCondition
    {
    public:
        explicit IsGroundedCondition( Blackboard& blackboard );

        bool evaluate( Blackboard& blackboard ) const override;

    };

    class IsIdleCondition final : public RigidBodyCondition
    {
    public:
        explicit IsIdleCondition( Blackboard& blackboard );

        bool evaluate( Blackboard& blackboard ) const override;

    };

    class IsMovingHorizontallyCondition final : public RigidBodyCondition
    {
    public:
        explicit IsMovingHorizontallyCondition( Blackboard& blackboard );

        bool evaluate( Blackboard& blackboard ) const override;

    };

}


#endif //!RIGIDBODYCONDITION_H
