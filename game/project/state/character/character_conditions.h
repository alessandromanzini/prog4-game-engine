#ifndef CHARACTER_CONDITIONS_H
#define CHARACTER_CONDITIONS_H

#include <framework/behaviour/fsm/transitions.h>


namespace game::condition
{
    struct IsAnimationCompletedCondition final : engine::fsm::Condition
    {
        bool evaluate( engine::Blackboard& blackboard ) const override;
    };


    struct IsRisingCondition final : engine::fsm::Condition
    {
        bool evaluate( engine::Blackboard& blackboard ) const override;
    };


    struct IsFallingCondition final : engine::fsm::Condition
    {
        bool evaluate( engine::Blackboard& blackboard ) const override;
    };


    struct IsGroundedCondition final : engine::fsm::Condition
    {
        bool evaluate( engine::Blackboard& blackboard ) const override;
    };


    struct IsMovingHorzCondition final : engine::fsm::Condition
    {
        bool evaluate( engine::Blackboard& blackboard ) const override;
    };

}


#endif //!CHARACTER_CONDITIONS_H
