#ifndef TRANSITIONS_H
#define TRANSITIONS_H

#include <framework/resources/data/Blackboard.h>


namespace engine
{
    class RigidBodyComponent;
}

namespace engine::fsm
{
    // +---------------------------+
    // | STATES                    |
    // +---------------------------+
    class State
    {
    public:
        virtual ~State( ) = default;

        State( const State& )                = delete;
        State( State&& ) noexcept            = delete;
        State& operator=( const State& )     = delete;
        State& operator=( State&& ) noexcept = delete;

        virtual void on_enter( Blackboard& ) { }
        virtual void on_exit( Blackboard& ) { }

        virtual void tick( Blackboard& ) { }
        virtual void render( Blackboard& ) const { }

    protected:
        State( ) = default;

    };

    // +---------------------------+
    // | CONDITIONS                |
    // +---------------------------+
    class Condition
    {
    public:
        explicit Condition( Blackboard& ) { }
        virtual ~Condition( ) = default;

        Condition( const Condition& )                = delete;
        Condition( Condition&& ) noexcept            = delete;
        Condition& operator=( const Condition& )     = delete;
        Condition& operator=( Condition&& ) noexcept = delete;

        virtual bool evaluate( Blackboard& blackboard ) const = 0;
    };

    namespace conditions
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

        class IsNotGroundedCondition final : public RigidBodyCondition
        {
        public:
            explicit IsNotGroundedCondition( Blackboard& blackboard );

            bool evaluate( Blackboard& blackboard ) const override;

        };

        class IsGroundedIdleCondition final : public RigidBodyCondition
        {
        public:
            explicit IsGroundedIdleCondition( Blackboard& blackboard );

            bool evaluate( Blackboard& blackboard ) const override;

        };

        class IsGroundedMovingCondition final : public RigidBodyCondition
        {
        public:
            explicit IsGroundedMovingCondition( Blackboard& blackboard );

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

        class IsFallingCondition final : public RigidBodyCondition
        {
        public:
            explicit IsFallingCondition( Blackboard& blackboard );

            bool evaluate( Blackboard& blackboard ) const override;

        };

    }

}


#endif //!TRANSITIONS_H
