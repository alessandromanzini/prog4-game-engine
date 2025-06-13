#include "ScoreComponent.h"

#include <singleton/GameTime.h>
#include <singleton/ResourceManager.h>
#include <singleton/ScenePool.h>

#include <fstream>
#include <string>

namespace game
{
    ScoreComponent::ScoreComponent( owner_t& owner )
        : Component{ owner }
    {
    }


    bool ScoreComponent::is_high_score( ) const
    {
        const auto path = engine::RESOURCE_MANAGER.get_data_path(  ) / "highscore.txt";
        int highScore{};
        if ( std::ifstream input{ path }; input.is_open( ) )
        {
            input >> highScore;
            input.close();
        }
        return score_ > highScore;
    }


    void ScoreComponent::update_high_score( ) const
    {
        const auto path = engine::RESOURCE_MANAGER.get_data_path(  ) / "highscore.txt";
        if ( std::ofstream output(path, std::ios::trunc); output.is_open( ) )
        {
            output << score_ << std::endl;
        }
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
            bool isHighScore = is_high_score( );
            if ( isHighScore )
            {
                update_high_score(  );
            }
            subject_.broadcast( engine::UID( ScoreEvents::GAMEOVER ), isHighScore );
            gameover_ = true;
            engine::GAME_TIME.set_timeout( 3.f, [scene = engine::SCENE_POOL.get_active_scene( ).get_name( )]
                {
                    engine::SCENE_POOL.unload_scene( scene );
                    engine::SCENE_POOL.select_first_scene( );
                } );
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
