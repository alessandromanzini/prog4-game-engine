#include <cassert>
#include <framework/behaviour/fsm/FiniteStateMachine.h>


namespace engine
{
    FiniteStateMachine::FiniteStateMachine( fsm::State* const startState, Blackboard& blackboard )
        : blackboard_ref_( blackboard )
    {
        change_state( startState );
    }


    void FiniteStateMachine::add_transition( fsm::State* const from, fsm::State* const to, const fsm::Condition* condition )
    {
        transitions_[from].push_back( std::make_pair( condition, to ) );
    }


    void FiniteStateMachine::tick( )
    {
        // Check the transitions map for a TransitionState pair
        if ( const auto it = transitions_.find( current_state_ptr_ ); it != transitions_.end( ) )
        {
            for ( auto& pairs = it->second; auto& [condition, state] : pairs )
            {
                if ( condition->evaluate( blackboard_ref_ ) )
                {
                    change_state( state );
                    break;
                }
            }
        }

        // Tick the (new or unchanged) state
        current_state_ptr_->tick( blackboard_ref_ );
    }


    void FiniteStateMachine::render( ) const
    {
        current_state_ptr_->render( blackboard_ref_ );
    }


    void FiniteStateMachine::change_state( fsm::State* const state )
    {
        if ( not state )
        {
            throw std::invalid_argument( "New state cannot be null!" );
        }

        // 1. On exit the current state (if any)
        if ( current_state_ptr_ != nullptr )
        {
            current_state_ptr_->on_exit( blackboard_ref_ );
        }

        // 2. Set the new state
        current_state_ptr_ = state;

        // 3. On enter the new state
        current_state_ptr_->on_enter( blackboard_ref_ );
    }


}
