#include "ScoreComponent.h"

#include <singleton/ResourceManager.h>
#include <singleton/ScenePool.h>


namespace game
{
    ScoreComponent::ScoreComponent( owner_t& owner )
        : Component{ owner }
    {
    }


    int ScoreComponent::get_score( ) const
    {
        return score_;
    }


    int ScoreComponent::get_lives( ) const
    {
        return player_lives_;
    }


    void ScoreComponent::increase_score( const int value )
    {
        if ( gameover_ )
        {
            return;
        }
        score_ += value;
        subject_.broadcast( engine::UID( ScoreEvents::SCORE_INCREASED ), score_ );
    }


    void ScoreComponent::signal_player_death( )
    {
        if ( gameover_ )
        {
            return;
        }
        if ( --player_lives_ < 0 )
        {
            subject_.broadcast( engine::UID( ScoreEvents::GAMEOVER ), score_ );
            gameover_ = true;
        }
        else
        {
            subject_.broadcast( engine::UID( ScoreEvents::PLAYER_DEATH ), player_lives_ );
        }
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
