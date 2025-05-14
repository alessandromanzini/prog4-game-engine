#ifndef FINITESTATEMACHINE_H
#define FINITESTATEMACHINE_H

#include <framework/behaviour/fsm/transitions.h>

#include <map>
#include <utility>
#include <vector>


namespace engine
{
    namespace fsm
    {
        struct StateStack
        {
            using transition_pair_t = std::pair<std::unique_ptr<Condition>, UID>;

            std::vector<std::unique_ptr<State>> states{};
            std::vector<transition_pair_t> transitions{};

            void trigger_on_enter( Blackboard& blackboard ) const;
            void trigger_on_exit( Blackboard& blackboard ) const;
            void trigger_tick( Blackboard& blackboard ) const;
        };
    }

    class Blackboard;
    class FiniteStateMachine final
    {
    public:
        explicit FiniteStateMachine( Blackboard& blackboard );

        /**
         * Create a state in the state machine linked to the UID. Multiple states can exist with the same UID, and they will all take action.
         * @tparam state_t Type of the state
         * @tparam state_args_t Arguments to be passed to the state constructor
         * @param state
         * @param args
         */
        template <typename state_t, typename... state_args_t>
            requires std::derived_from<state_t, fsm::Condition>
                     && std::constructible_from<state_t, state_args_t...>
        void create_state( UID state, state_args_t&&... args );

        template <typename condition_t> requires std::derived_from<condition_t, fsm::Condition>
        void add_transition( UID from, UID to );

        void tick( );

    private:
        void change_state( UID state );

        Blackboard& blackboard_ref_;

        std::map<UID, fsm::StateStack> stacks_{};

        UID current_state_id_{ NULL_UID };

    };


    template <typename state_t, typename... state_args_t>
        requires std::derived_from<state_t, fsm::Condition>
                 && std::constructible_from<state_t, state_args_t...>
    void FiniteStateMachine::create_state( const UID state, state_args_t&&... args )
    {
        stacks_[state].states.push_back( std::make_unique<state_t>( std::forward<state_args_t...>( args... ) ) );

        // Lazy initialization of the state machine to assign the first state
        if ( current_state_id_ == NULL_UID )
        {
            change_state( state );
        }
    }


    template <typename condition_t> requires std::derived_from<condition_t, fsm::Condition>
    void FiniteStateMachine::add_transition( const UID from, const UID to )
    {
        stacks_[from].transitions.push_back( std::make_pair( std::make_unique<condition_t>( ), to ) );
    }

}


#endif //!FINITESTATEMACHINE_H
