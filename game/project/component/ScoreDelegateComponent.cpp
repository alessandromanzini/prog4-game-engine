#include "ScoreDelegateComponent.h"

#include "ScoreComponent.h"

#include <cassert>


namespace game
{
    ScoreDelegateComponent::ScoreDelegateComponent( owner_t& owner, ScoreComponent* score )
        : Component( owner )
        , score_component_ptr_{ score }
    {
        assert( score_component_ptr_ && "ScoreDelegateComponent: ScoreComponent pointer cannot be nullptr!" );
    }


    void ScoreDelegateComponent::increase_score( const int value ) const
    {
        score_component_ptr_->increase_score( value );
    }


    void ScoreDelegateComponent::signal_player_death( ) const
    {
        score_component_ptr_->signal_player_death( );
    }


}
