#ifndef RIGIDBODYCONDITION_H
#define RIGIDBODYCONDITION_H

#include <framework/behaviour/fsm/__transition/base_transitions.h>


namespace engine
{
    class RigidBodyComponent;
}

namespace engine::fsm::condition
{
    class RigidBodyCondition : public Condition
    {
    public:
        bool is_idle( const Blackboard& blackboard ) const;
        bool is_moving( const Blackboard& blackboard ) const;

        bool is_falling( const Blackboard& blackboard ) const;
        bool is_grounded( const Blackboard& blackboard ) const;

    protected:
        [[nodiscard]] const RigidBodyComponent& get_rigid_body( const Blackboard& blackboard ) const;

    private:
        static constexpr float IDLE_THRESHOLD_ = 0.1f;

    };

    struct IsGroundedCondition final : RigidBodyCondition
    {
        bool evaluate( Blackboard& blackboard ) const override;
    };

    struct IsIdleCondition final : RigidBodyCondition
    {
        bool evaluate( Blackboard& blackboard ) const override;
    };

    struct IsMovingHorizontallyCondition final : RigidBodyCondition
    {
        bool evaluate( Blackboard& blackboard ) const override;
    };

    struct IsFallingCondition final : RigidBodyCondition
    {
        bool evaluate( Blackboard& blackboard ) const override;
    };

}


#endif //!RIGIDBODYCONDITION_H
