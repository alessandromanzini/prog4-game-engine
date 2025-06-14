#include "GameOverComponent.h"

#include <fstream>
#include <framework/component/TextComponent.h>
#include <framework/component/TextureComponent.h>
#include <singleton/GameInstance.h>
#include <singleton/GameTime.h>
#include <singleton/ResourceManager.h>
#include <singleton/ScenePool.h>

#include "ScoreComponent.h"


namespace game
{
    GameOverComponent::GameOverComponent( owner_t& owner, const std::shared_ptr<engine::Font>& font )
        : Component{ owner }
    {
        gameover_text_object_ptr_ = &get_owner( ).create_child( );
        gameover_text_object_ptr_->add_component<engine::TextComponent>( "", font, true );
        gameover_text_object_ptr_->set_world_transform(
            engine::Transform::from_translation( engine::GAME_INSTANCE.get_screen_dimensions( ) / 2.f ) );
    }


    void GameOverComponent::notify( const engine::UID event, const engine::event::broadcast_value_variant_t value )
    {
        if ( event == engine::UID( ScoreEvents::GAMEOVER ) && not is_gameover_ )
        {
            is_gameover_ = true;
            gameover_text_object_ptr_->get_component<engine::TextComponent>( ).value( ).set_text( "GAMEOVER" );

            if ( const auto score = std::get<int>( value ); is_high_score( score ) )
            {
                update_high_score( score );
                // todo> display high score message
            }

            engine::GAME_TIME.set_timeout( 3.f, [scene = engine::SCENE_POOL.get_active_scene( ).get_name( )]
                                               {
                                                   engine::SCENE_POOL.unload_scene( scene );
                                                   engine::SCENE_POOL.select_first_scene( );
                                               } );

            //auto& gameOverObject = scene.create_object( );
            //gameOverObject.add_component<engine::TextureComponent>( "backgrounds/game_over.png" );
            //gameOverObject.add_component<engine::AudioComponent>( "audio/game_over.wav", false, true );
        }
    }


    bool GameOverComponent::is_high_score( const int score ) const
    {
        const auto path = engine::RESOURCE_MANAGER.get_data_path( ) / "highscore.txt";
        int highScore{};
        if ( std::ifstream input{ path }; input.is_open( ) )
        {
            input >> highScore;
            input.close( );
        }
        return score > highScore;
    }


    void GameOverComponent::update_high_score( const int score ) const
    {
        const auto path = engine::RESOURCE_MANAGER.get_data_path( ) / "highscore.txt";
        if ( std::ofstream output( path, std::ios::trunc ); output.is_open( ) )
        {
            output << score << std::endl;
        }
    }

}
