#ifndef COMBINE_H
#define COMBINE_H

#include <framework/behaviour/fsm/__transition/base_transitions.h>


namespace engine::fsm::condition
{
    template <typename logic_t, typename... conditions_t>
                requires ( std::derived_from<conditions_t, Condition> && ... )
            class Combine final : public Condition
    {
    public:
        Combine( );
        bool evaluate( Blackboard& blackboard ) const override;

    private:
        std::vector<std::unique_ptr<Condition>> conditions_{};
        std::vector<Condition*> raw_{}; // TODO: change this

    };

    template <typename logic_t, typename ... conditions_t> requires (std::derived_from<conditions_t, Condition> && ...)
        Combine<logic_t, conditions_t...>::Combine( )
    {
        ( conditions_.push_back( std::make_unique<conditions_t>( ) ), ... );
        raw_.reserve( conditions_.size( ) );
        for ( auto& condition : conditions_ )
        {
            raw_.push_back( condition.get( ) );
        }
    }


    template <typename logic_t, typename ... conditions_t> requires (std::derived_from<conditions_t, Condition> && ...)
    bool Combine<logic_t, conditions_t...>::evaluate( Blackboard& blackboard ) const
    {
        return logic_t{}( blackboard, raw_ );
    }
}


#endif //!COMBINE_H
