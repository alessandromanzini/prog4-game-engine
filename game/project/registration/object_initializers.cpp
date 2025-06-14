#include "object_initializers.h"

#include <framework/GameObject.h>
#include <framework/component/physics/BoxColliderComponent.h>
#include <framework/component/physics/PhysicsComponent.h>

#include <command/BubbleAttackCommand.h>
#include <command/EmptyAttackCommand.h>
#include <command/RockAttackCommand.h>

#include <controller/CharacterController.h>

#include <singleton/ResourceManager.h>

#include <registration/tags.h>

#include <component/BubbleCaptureComponent.h>
#include <component/CharacterComponent.h>
#include <component/CollisionsComponent.h>
#include <component/FruitComponent.h>
#include <component/GameOverComponent.h>
#include <component/GameOverDelegateComponent.h>
#include <component/JoinMenuComponent.h>
#include <component/LevelLoaderComponent.h>
#include <component/MaitaComponent.h>
#include <component/ScoreComponent.h>
#include <component/ScoreDelegateComponent.h>
#include <component/StatsDisplayComponent.h>
#include <component/ZenChanComponent.h>
#include <framework/Scene.h>
#include <framework/component/AudioComponent.h>
#include <framework/component/TextComponent.h>
#include <framework/component/TextureComponent.h>

#include "audio.h"
#include "object_stats.h"


namespace game
{
    void create_character( engine::GameObject& object, ScoreComponent* score, const bool isBub )
    {
        object.set_tag( engine::UID( ObjectTags::ALLY ) );

        auto& body = object.add_component<engine::PhysicsComponent>( );
        body.set_simulate_physics( true );

        object.add_component<engine::BoxColliderComponent>( glm::vec2{ 24.f, 40.f }, glm::vec2{ 8.f, 0.f } );
        object.add_component<CollisionsComponent>( );

        auto& projectileSocket = object.create_child( );
        projectileSocket.set_local_transform( engine::Transform::from_translation( { 20.f, 22.f } ) );

        object.add_component<CharacterComponent>( isBub ? stats::get_bub_resources( ) : stats::get_bob_resources( ),
                                                  std::make_unique<BubbleAttackCommand>( object, projectileSocket ),
                                                  std::make_unique<JumpCommand>( object, stats::get_character_jump_force( ) ),
                                                  std::make_unique<MoveCommand>( object, stats::get_character_speed( ) ),
                                                  true );

        if ( score )
        {
            object.add_component<ScoreDelegateComponent>( score );
        }
    }


    void create_join_menu( engine::GameObject& join )
    {
        const auto font = engine::RESOURCE_MANAGER.load_font( "fonts/pixelify.ttf", 36 );

        join.add_component<engine::TextureComponent>( "maps/main_menu.png" );

        auto& player1 = join.create_child( );
        player1.set_world_transform( engine::Transform::from_translation( { 530.f, 15.f } ) );
        player1.add_component<engine::TextComponent>( "P1", font );

        auto& player2 = join.create_child( );
        player2.set_world_transform( engine::Transform::from_translation( { 530.f, 50.f } ) );
        player2.add_component<engine::TextComponent>( "P2", font );

        auto& selection = join.create_child( );

        auto& arcade = selection.create_child( );
        arcade.add_component<engine::TextComponent>( "arcade", font );
        arcade.set_world_transform( engine::Transform::from_translation( { 130.f, 410.f } ) );
        arcade.set_tag( engine::UID( ObjectTags::ARCADE ) );

        auto& versus = selection.create_child( );
        versus.add_component<engine::TextComponent>( "versus", font );
        versus.set_world_transform( engine::Transform::from_translation( { 370.f, 410.f } ) );
        versus.set_tag( engine::UID( ObjectTags::VERSUS ) );

        auto& audio = join.add_component<engine::AudioComponent>( "theme.mp3", engine::sound::SoundType::SOUND_TRACK );
        audio.set_volume( .25f );
        audio.play( -1 );

        join.add_component<JoinMenuComponent>( &selection, std::vector{ &player1, &player2 } );
    }


    void create_bub( engine::GameObject& object, ScoreComponent* score )
    {
        create_character( object, score, true );
    }


    void create_bob( engine::GameObject& object, ScoreComponent* score )
    {
        create_character( object, score, false );
    }


    void create_zenchan( engine::GameObject& object, const glm::vec2 position, std::vector<engine::GameObject*> targets )
    {
        object.set_tag( engine::UID( ObjectTags::ENEMY ) );
        object.set_local_transform( engine::Transform::from_translation( position ) );

        auto& body = object.add_component<engine::PhysicsComponent>( );
        body.set_simulate_physics( true );

        object.add_component<engine::BoxColliderComponent>( glm::vec2{ 24.f, 32.f }, glm::vec2{ 4.f, 0.f } );
        object.add_component<CollisionsComponent>( );

        object.add_component<CharacterComponent>( stats::get_zen_chan_resources( ),
                                                  std::make_unique<EmptyAttackCommand>( object ),
                                                  std::make_unique<JumpCommand>( object, stats::get_zen_chan_jump_force( ) ),
                                                  std::make_unique<MoveCommand>( object, stats::get_zen_chan_speed( ) ),
                                                  false );
        object.add_component<BubbleCaptureComponent>( stats::get_zen_chan_capture_resources( ) );
        object.add_component<ZenChanComponent>( ).set_targets( std::move( targets ) );
    }


    void create_maita( engine::GameObject& object, const glm::vec2 position, std::vector<engine::GameObject*> targets )
    {
        object.set_tag( engine::UID( ObjectTags::ENEMY ) );
        object.set_local_transform( engine::Transform::from_translation( position ) );

        auto& body = object.add_component<engine::PhysicsComponent>( );
        body.set_simulate_physics( true );

        object.add_component<engine::BoxColliderComponent>( glm::vec2{ 24.f, 32.f }, glm::vec2{ 4.f, 0.f } );
        object.add_component<CollisionsComponent>( );

        auto& projectileSocket = object.create_child( );
        projectileSocket.set_local_transform( engine::Transform::from_translation( { 20.f, 10.f } ) );

        object.add_component<CharacterComponent>( stats::get_maita_resources( ),
                                                  std::make_unique<RockAttackCommand>( object, projectileSocket ),
                                                  std::make_unique<JumpCommand>( object, stats::get_maita_jump_force( ) ),
                                                  std::make_unique<MoveCommand>( object, stats::get_maita_speed( ) ),
                                                  false );
        object.add_component<BubbleCaptureComponent>( stats::get_maita_capture_resources( ) );

        if ( not targets.empty( ) )
        {
            object.add_component<MaitaComponent>( ).set_targets( std::move( targets ) );
        }
    }


    void create_fruit( engine::GameObject& object, const std::string& texturePath, int value, const glm::vec2 position,
                       const glm::vec2 spawnerPosition )
    {
        object.set_local_transform( engine::Transform::from_translation( position ) );
        object.set_tag( engine::UID( ObjectTags::FRUIT ) );

        object.add_component<engine::TextureComponent>( texturePath );
        object.add_component<engine::BoxColliderComponent>( glm::vec2{ 36.f, 30.f } );
        object.add_component<CollisionsComponent>( );
        object.add_component<FruitComponent>( value, 5, glm::normalize( position - spawnerPosition ) );
        object.add_component<engine::AudioComponent>( "characters/sfx/fruit_capture.wav",
                                                      engine::sound::SoundType::SOUND_EFFECT,
                                                      engine::UID( AudioCue::SFX ) ).set_playback_on_deletion( );
    }


    void create_score( engine::GameObject& object, std::shared_ptr<engine::Font> font )
    {
        auto& display = object.add_component<StatsDisplayComponent>( font );

        auto& score = object.add_component<ScoreComponent>( );
        score.add_observer( display );
    }


    void create_arcade( engine::Scene& scene, const std::vector<CharacterController*>& controllers,
                        const std::shared_ptr<engine::Font>& font )
    {
        auto& gameover = scene.create_object( );
        auto& gameoverComponent = gameover.add_component<GameOverComponent>( font );

        auto& score = scene.create_object( );
        score.set_world_transform( engine::Transform::from_translation( { 45.f, 15.f } ) );
        create_score( score, font );
        score.get_component<ScoreComponent>(  ).value(  ).add_observer( gameoverComponent );

        std::vector<engine::GameObject*> characters{};
        bool alt{};
        for ( auto* controller : controllers )
        {
            if ( controller->has_joined( ) )
            {
                controller->set_gameover_component( &gameoverComponent );
                engine::GameObject& character = scene.create_object( );
                if ( not alt )
                {
                    create_bub( character, &score.get_component<ScoreComponent>( ) );
                }
                else
                {
                    create_bob( character, &score.get_component<ScoreComponent>( ) );
                }
                controller->possess( &character );
                characters.emplace_back( &character );
                alt = !alt;
            }
            controller->set_block_selection( true );
        }

        auto& levelLoader = scene.create_object( );
        levelLoader.add_component<LevelLoaderComponent>( std::move( characters ),
                                                         std::vector{ "maps/level1.csv", "maps/level2.csv", "maps/level3.csv" },
                                                         false, true );
        levelLoader.set_tag( engine::UID( ObjectTags::LEVEL_LOADER ) );
    }


    void create_versus( engine::Scene& scene, const std::vector<CharacterController*>& controllers,
                        const std::shared_ptr<engine::Font>& font )
    {
        auto& gameover = scene.create_object( );
        auto& gameoverObserver = gameover.add_component<GameOverComponent>( font, false );

        std::vector<engine::GameObject*> characters{};
        bool alt{};
        for ( auto* controller : controllers )
        {
            if ( controller->has_joined( ) )
            {
                engine::GameObject& character = scene.create_object( );
                if ( not alt )
                {
                    create_bub( character, nullptr );
                }
                else
                {
                    create_maita( character, {} );
                }
                character.add_component<GameOverDelegateComponent>( ).add_observer( gameoverObserver );
                controller->possess( &character );
                characters.emplace_back( &character );
                alt = !alt;
            }
            controller->set_block_selection( true );
        }

        auto& levelLoader = scene.create_object( );
        levelLoader.add_component<LevelLoaderComponent>( std::move( characters ),
                                                         std::vector{ "maps/level1.csv", "maps/level2.csv", "maps/level3.csv" },
                                                         true, false );
        levelLoader.set_tag( engine::UID( ObjectTags::LEVEL_LOADER ) );
    }

}
