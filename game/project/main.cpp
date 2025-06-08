#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
		#include <vld.h>
#endif
#endif

#define SDL_MAIN_HANDLED

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <core/Minigin.h>

#include <framework/GameObject.h>
#include <framework/Scene.h>
#include <framework/component/AudioComponent.h>
#include <framework/component/TextComponent.h>
#include <framework/component/physics/BoxColliderComponent.h>
#include <framework/component/physics/PhysicsComponent.h>

#include <singleton/GameInstance.h>
#include <singleton/ResourceManager.h>
#include <singleton/ScenePool.h>
#include <singleton/ServiceLocator.h>

#include <registration/audio.h>
#include <registration/input.h>
#include <registration/tags.h>

#include <component/CharacterComponent.h>

#include <controller/CharacterController.h>
#include <controller/DebugController.h>

#include <component/CollisionsComponent.h>
#include <framework/component/TextureComponent.h>

#include <filesystem>
namespace fs = std::filesystem;


void create_bub( engine::GameObject& object )
{
    constexpr float PROJECTILE_SPEED{ 100.f };
    constexpr float PROJECTILE_OFFSET{ 20.f };
    constexpr float JUMP_FORCE{ 300.f };
    constexpr float MOVEMENT_SPEED{ 80.f };

    object.set_tag( engine::UID( game::ObjectTags::ALLY ) );
    object.set_world_transform( engine::Transform::from_translation( { 200.f, 375.f } ) );

    auto& body = object.add_component<engine::PhysicsComponent>( );
    body.set_simulate_physics( true );

    object.add_component<engine::BoxColliderComponent>( glm::vec2{ 24.f, 40.f }, glm::vec2{ 8.f, 0.f } );
    object.add_component<game::CollisionsComponent>( );

    game::CharacterResources resources{
        .idle_sprite = engine::Sprite2D{
            "characters/bub/bub_idle_1x8.png",
            static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 8u ),
            0.2f, 2.f
        },
        .walk_sprite = engine::Sprite2D{
            "characters/bub/bub_walk_1x6.png",
            static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 6u ),
            0.2f, 2.f,
            { 0.f, -4.f }
        },
        .rise_sprite = engine::Sprite2D{
            "characters/bub/bub_jump_1x2.png",
            static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 2u ),
            0.2f, 2.f
        },
        .fall_sprite = engine::Sprite2D{
            "characters/bub/bub_fall_1x1.png",
            static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 1u ),
            0.2f, 2.f
        },
        .attack_sprite = engine::Sprite2D{
            "characters/bub/bub_attack_1x3.png",
            static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 3u ),
            .1f, 2.f, {}, false
        },
        .attack_audio_path = "characters/sfx/character_attack.wav",
        .jump_audio_path = "characters/sfx/character_jump.wav"
    };

    auto& projectileSocket = object.create_child( );
    projectileSocket.set_local_transform( engine::Transform::from_translation( { 20.f, 20.f } ) );

    object.add_component<game::CharacterComponent>( resources,
        std::make_unique<game::AttackCommand>( object, projectileSocket, PROJECTILE_SPEED, PROJECTILE_OFFSET ),
        std::make_unique<game::JumpCommand>( object, JUMP_FORCE ),
        std::make_unique<game::MoveCommand>( object, MOVEMENT_SPEED ) );
}


void create_grid( engine::GameObject& object )
{
    object.add_component<engine::TextureComponent>( "maps/level1_layout.png" );
    object.add_component<engine::BoxColliderComponent>( glm::vec2{ 640.f, 25.f } );
    object.add_component<engine::BoxColliderComponent>( glm::vec2{ 50.f, 480.f } );
    object.add_component<engine::BoxColliderComponent>( glm::vec2{ 640.f, 25.f }, glm::vec2{ 0.f, 465.f } );
    object.add_component<engine::BoxColliderComponent>( glm::vec2{ 50.f, 480.f }, glm::vec2{ 600.f, 0.f } );

    auto& platformsObject = object.create_child( );
    platformsObject.set_tag( engine::UID( game::ObjectTags::PLATFORM ) );
    platformsObject.add_component<engine::BoxColliderComponent>( glm::vec2{ 40.f, 20.f }, glm::vec2{ 40.f, 370.f } );
    platformsObject.add_component<engine::BoxColliderComponent>( glm::vec2{ 360.f, 20.f }, glm::vec2{ 140.f, 370.f } );
    platformsObject.add_component<engine::BoxColliderComponent>( glm::vec2{ 40.f, 20.f }, glm::vec2{ 560.f, 370.f } );

    platformsObject.add_component<engine::BoxColliderComponent>( glm::vec2{ 40.f, 20.f }, glm::vec2{ 40.f, 279.f } );
    platformsObject.add_component<engine::BoxColliderComponent>( glm::vec2{ 360.f, 20.f }, glm::vec2{ 140.f, 279.f } );
    platformsObject.add_component<engine::BoxColliderComponent>( glm::vec2{ 40.f, 20.f }, glm::vec2{ 560.f, 279.f } );

    platformsObject.add_component<engine::BoxColliderComponent>( glm::vec2{ 40.f, 20.f }, glm::vec2{ 40.f, 185.f } );
    platformsObject.add_component<engine::BoxColliderComponent>( glm::vec2{ 360.f, 20.f }, glm::vec2{ 140.f, 185.f } );
    platformsObject.add_component<engine::BoxColliderComponent>( glm::vec2{ 40.f, 20.f }, glm::vec2{ 560.f, 185.f } );
}


void load( )
{
    game::initialize_input( );
    game::initialize_audio( );

    engine::GAME_INSTANCE.set_gravity_coefficient( 400.f );

    const auto font = engine::RESOURCE_MANAGER.load_font( "fonts/Lingua.otf", 36 );
    auto& scene     = engine::SCENE_POOL.create_scene( "Demo" );

    // Bub
    auto& bub = scene.create_object( );
    create_bub( bub );

    auto& grid = scene.create_object( );
    create_grid( grid );

#ifndef NDEBUG
    engine::GAME_INSTANCE.add_player_controller<game::DebugController>( );
#endif

    auto& controller = engine::GAME_INSTANCE.add_player_controller<game::CharacterController>( );
    controller.possess( &bub );
}


int main( int, char*[] )
{
#if __EMSCRIPTEN__
	fs::path dataLocation = "";
#else
    fs::path dataLocation = "./resources/";
    if ( !exists( dataLocation ) )
    {
        dataLocation = "../resources/";
    }
#endif
    engine::Minigin engine{ dataLocation };
    engine.run( load );
    return 0;
}
