#include "GameOverComponent.h"

#include <fstream>
#include <framework/component/TextComponent.h>
#include <helpers/CsvReader.h>
#include <singleton/GameInstance.h>
#include <singleton/GameTime.h>
#include <singleton/ResourceManager.h>
#include <singleton/ScenePool.h>

#include "ScoreComponent.h"


namespace game
{
    GameOverComponent::GameOverComponent( owner_t& owner, const std::shared_ptr<engine::Font>& font, const bool showHighScores )
        : Component{ owner }
        , show_high_scores_{ showHighScores }
        , font_ptr_{ font } { }


    void GameOverComponent::tick( )
    {
        if ( is_gameover_ && not waiting_for_redirect_ )
        {
            if ( not needs_name_input_ )
            {
                update_high_scores( scores_ );
                engine::GAME_TIME.set_timeout( 3.f, [scene = engine::SCENE_POOL.get_active_scene( ).get_name( )]
                                                   {
                                                       engine::SCENE_POOL.unload_scene( scene );
                                                       engine::SCENE_POOL.select_first_scene( );
                                                   } );
                waiting_for_redirect_ = true;
            }
        }
    }


    void GameOverComponent::notify( const engine::UID event, const engine::event::broadcast_value_variant_t value )
    {
        if ( event == engine::UID( ScoreEvents::GAMEOVER ) && not is_gameover_ )
        {
            is_gameover_ = true;
            manage_high_score( std::get<int>( value ) );
        }
    }


    void GameOverComponent::letter_vert( const bool up )
    {
        if ( needs_name_input_ )
        {
            int val = scores_[score_index_].first[letter_index_] + ( up ? 1 : -1 );
            if ( val < 'A' )
            {
                val = 'Z';
            }
            else if ( val > 'Z' )
            {
                val = 'A';
            }
            scores_[score_index_].first[letter_index_] = static_cast<char>( val );
            display_scores( );
        }
    }


    void GameOverComponent::letter_horz( const bool left )
    {
        if ( needs_name_input_ )
        {
            letter_index_ = static_cast<size_t>( std::clamp( static_cast<int>( letter_index_ ) + ( left ? -1 : 1 ), 0, 3 ) );
        }
    }


    void GameOverComponent::confirm( )
    {
        needs_name_input_ = false;
    }


    void GameOverComponent::manage_high_score( const int score )
    {
        scores_ = get_high_scores( );
        if ( score > 0 )
        {
            scores_.push_back( { "_", score } );
            needs_name_input_ = true;
        }

        // Order and clamp high scores
        std::ranges::sort( scores_, []( const auto& lhs, const auto& rhs )
                               {
                                   return lhs.second > rhs.second; // sort descending by score
                               } );
        if ( scores_.size( ) > MAX_HIGH_SCORES_ )
        {
            scores_.resize( MAX_HIGH_SCORES_ );
        }

        const auto ownScore = std::ranges::find_if( scores_, []( const auto& score ) { return score.first == "_"; } );
        needs_name_input_   = ownScore != scores_.end( );
        score_index_        = std::ranges::distance( scores_.begin( ), ownScore );
        if ( needs_name_input_ )
        {
            ownScore->first = "AAA";
        }

        display_scores( );
    }


    std::vector<std::pair<std::string, int>> GameOverComponent::get_high_scores( ) const
    {
        const auto path = engine::RESOURCE_MANAGER.get_data_path( ) / "highscores.csv";
        CsvReader reader{ path };

        std::vector<std::pair<std::string, int>> scores{};
        while ( not reader.eof( ) )
        {
            scores.push_back( { reader.get( 0 ), reader.get_int( 1 ) } );
            reader.next( );
        }
        return scores;
    }


    void GameOverComponent::update_high_scores( const std::vector<std::pair<std::string, int>>& scores ) const
    {
        const auto path = engine::RESOURCE_MANAGER.get_data_path( ) / "highscores.csv";
        std::ofstream file{ path };
        for ( const auto& [name, score] : scores )
        {
            file << name << "; " << score << ";\n";
        }
    }


    void GameOverComponent::display_scores( )
    {
        if ( gameover_text_object_ptr_ != nullptr )
        {
            gameover_text_object_ptr_->mark_for_deletion( );
        }

        gameover_text_object_ptr_ = &get_owner( ).create_child( );
        auto& text                = gameover_text_object_ptr_->add_component<engine::TextComponent>(
            needs_name_input_ ? "GAMEOVER - HIGHSCORE" : "GAMEOVER", font_ptr_, true );
        gameover_text_object_ptr_->set_world_transform(
            engine::Transform::from_translation( { engine::GAME_INSTANCE.get_screen_dimensions( ).x / 2.f, 100.f } ) );
        text.commit( );
        int i{};
        for ( auto& [name, score] : scores_ )
        {
            std::stringstream ss{};
            ss << name << " : " << score;
            auto& child = gameover_text_object_ptr_->create_child( );
            child.add_component<engine::TextComponent>( ss.str( ), font_ptr_, true );
            child.set_local_transform( engine::Transform::from_translation( { 0.f, text.get_text_size( ).y * ( i + 1 ) } ) );
            ++i;
        }
    }

}
