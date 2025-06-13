#include "ScoreComponent.h"


namespace game
{
    ScoreComponent::ScoreComponent( owner_t& owner )
        : Component{ owner }
    {
    }


    bool ScoreComponent::is_high_score( ) const
    {
        // todo: implement high score logic
        return true;
    }


    int ScoreComponent::get_score( ) const
    {
        return score_;
    }


    void ScoreComponent::increase_score( const int value )
    {
        score_ += value;
        subject_.broadcast( engine::UID( ScoreEvents::SCORE_INCREASED ), score_ );
    }


    void ScoreComponent::signal_player_death( )
    {
        player_lives_ -= 1;
        subject_.broadcast( engine::UID( ScoreEvents::PLAYER_DEATH ), player_lives_ );
    }


    void ScoreComponent::add_observer( engine::Observer& observer )
    {
        subject_.add_observer( observer );
        subject_.broadcast( engine::UID( ScoreEvents::SCORE_INCREASED ), score_ );
        subject_.broadcast( engine::UID( ScoreEvents::PLAYER_DEATH ), player_lives_ );
    }


    void ScoreComponent::remove_observer( const engine::Observer& observer )
    {
        subject_.remove_observer( observer );
    }

}
