#ifndef LOGIC_H
#define LOGIC_H

#include <framework/resource/data/Blackboard.h>

#include <framework/behaviour/fsm/__transition/base_transitions.h>


namespace engine::fsm::logic
{
    struct And
    {
        bool operator()( Blackboard& blackboard, const std::vector<Condition*>& conditions ) const
        {
            for ( const auto& condition : conditions )
            {
                if ( not condition->evaluate( blackboard ) )
                {
                    return false;
                }
            }
            return true;
        }
    };

    struct Or
    {
        bool operator()( Blackboard& blackboard, const std::vector<Condition*>& conditions ) const
        {
            for ( const auto& condition : conditions )
            {
                if ( condition->evaluate( blackboard ) )
                {
                    return true;
                }
            }
            return false;
        }
    };

    struct NotAnd
    {
        bool operator()( Blackboard& blackboard, const std::vector<Condition*>& conditions ) const
        {
            return not And{}( blackboard, conditions );
        }
    };

    struct NotOr
    {
        bool operator()( Blackboard& blackboard, const std::vector<Condition*>& conditions ) const
        {
            return not Or{}( blackboard, conditions );
        }
    };
}


#endif //!LOGIC_H
