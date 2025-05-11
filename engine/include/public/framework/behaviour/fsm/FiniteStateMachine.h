#ifndef FINITESTATEMACHINE_H
#define FINITESTATEMACHINE_H

#include <framework/behaviour/fsm/transitions.h>

#include <map>
#include <utility>
#include <vector>


namespace engine
{
    class Blackboard;
    class FiniteStateMachine final
    {
        using transition_pair_t = std::pair<const fsm::Condition*, fsm::State*>;
        using transitions_t     = std::vector<transition_pair_t>;

    public:
        explicit FiniteStateMachine( fsm::State* startState, Blackboard& blackboard );

        void add_transition( fsm::State* from, fsm::State* to, const fsm::Condition* condition );

        void tick( );
        void render( ) const;

    private:
        void change_state( fsm::State* state );

        Blackboard& blackboard_ref_;

        std::map<fsm::State*, transitions_t> transitions_{};
        fsm::State* current_state_ptr_{ nullptr };

    };

}


#endif //!FINITESTATEMACHINE_H
