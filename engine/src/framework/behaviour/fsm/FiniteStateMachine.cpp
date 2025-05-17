#include <framework/behaviour/fsm/FiniteStateMachine.h>

#include <cassert>
#include <stdexcept>


namespace engine
{
    // +---------------------------+
    // | STATE STACK               |
    // +---------------------------+
    void fsm::StateStack::trigger_on_enter( Blackboard& blackboard ) const
    {
        for ( auto& state : states )
        {
            state->on_enter( blackboard );
        }
    }


    void fsm::StateStack::trigger_on_exit( Blackboard& blackboard ) const
    {
        for ( auto& state : states )
        {
            state->on_exit( blackboard );
        }
    }


    void fsm::StateStack::trigger_tick( Blackboard& blackboard ) const
    {
        for ( auto& state : states )
        {
            state->tick( blackboard );
        }
    }


    // +---------------------------+
    // | FINITE STATE MACHINE      |
    // +---------------------------+
    FiniteStateMachine::FiniteStateMachine( Blackboard& blackboard )
        : blackboard_ref_{ blackboard }
    {
    }


    void FiniteStateMachine::start( const UID startStateId )
    {
        assert( current_state_id_ == NULL_UID && "FiniteStateMachine::start: State machine already started!" );
        change_state( startStateId );
    }


    void FiniteStateMachine::tick( )
    {
        // Check the transitions map for a TransitionState pair
        if ( const auto it = stacks_.find( current_state_id_ ); it != stacks_.end( ) )
        {
            for ( auto& stack = it->second; auto& [condition, state] : stack.transitions )
            {
                if ( condition->evaluate( blackboard_ref_ ) )
                {
                    change_state( state );
                    break;
                }
            }
        }

        // Tick the (new or unchanged) state
        stacks_.at( current_state_id_ ).trigger_tick( blackboard_ref_ );
    }


    void FiniteStateMachine::change_state( const UID uid )
    {
        assert( stacks_.contains( uid ) && "FiniteStateMachine::change_state: Invalid state!" );

        if ( current_state_id_ == uid )
        {
            return;
        }

        // 1. On exit the current state (if any)
        if ( current_state_id_ != NULL_UID )
        {
            stacks_.at( current_state_id_ ).trigger_on_exit( blackboard_ref_ );
        }

        // 2. Set the new state
        current_state_id_ = uid;

        // 3. On enter the new state
        stacks_.at( current_state_id_ ).trigger_on_enter( blackboard_ref_ );
    }


}
