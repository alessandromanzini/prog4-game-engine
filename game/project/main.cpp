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

#include <filesystem>
#include <component/CollisionsComponent.h>
#include <framework/component/TextureComponent.h>
namespace fs = std::filesystem;


void create_bub( engine::GameObject& object )
{
    object.set_tag( engine::UID( game::ObjectTags::ALLY ) );
    object.set_world_transform( engine::Transform::from_translation( { 200.f, 375.f } ) );

    auto& body = object.add_component<engine::PhysicsComponent>( );
    body.set_simulate_physics( true );

    object.add_component<engine::BoxColliderComponent>( glm::vec2{ 24.f, 40.f }, glm::vec2{ 8.f, 0.f } );
    object.add_component<game::CollisionsComponent>( );

    auto& projectileSocket = object.create_child( );
    projectileSocket.set_local_transform( engine::Transform::from_translation( { 20.f, 20.f } ) );
    object.add_component<game::CharacterComponent>( projectileSocket );

    object.add_component<engine::AudioComponent>( "victory.wav", engine::sound::SoundType::SOUND_EFFECT,
                                                  engine::UID( game::AudioCue::GENERAL ) );
    object.add_component<engine::AudioComponent>( "pacdeath.wav", engine::sound::SoundType::SOUND_EFFECT,
                                                  engine::UID( game::AudioCue::GENERAL ) ).set_playback_on_deletion( );
}


void create_grid( engine::GameObject& object )
{
    object.add_component<engine::TextureComponent>( "maps/level1_layout.png" );
    object.add_component<engine::BoxColliderComponent>( glm::vec2{ 640.f, 25.f } );
    object.add_component<engine::BoxColliderComponent>( glm::vec2{ 50.f, 480.f } );
    object.add_component<engine::BoxColliderComponent>( glm::vec2{ 640.f, 25.f }, glm::vec2{ 0.f, 465.f } );
    object.add_component<engine::BoxColliderComponent>( glm::vec2{ 50.f, 480.f }, glm::vec2{ 600.f, 0.f } );

    object.add_component<engine::BoxColliderComponent>( glm::vec2{ 40.f, 20.f }, glm::vec2{ 40.f, 370.f } );
    object.add_component<engine::BoxColliderComponent>( glm::vec2{ 360.f, 20.f }, glm::vec2{ 140.f, 370.f } );
    object.add_component<engine::BoxColliderComponent>( glm::vec2{ 40.f, 20.f }, glm::vec2{ 560.f, 370.f } );

    object.add_component<engine::BoxColliderComponent>( glm::vec2{ 40.f, 20.f }, glm::vec2{ 40.f, 279.f } );
    object.add_component<engine::BoxColliderComponent>( glm::vec2{ 360.f, 20.f }, glm::vec2{ 140.f, 279.f } );
    object.add_component<engine::BoxColliderComponent>( glm::vec2{ 40.f, 20.f }, glm::vec2{ 560.f, 279.f } );

    object.add_component<engine::BoxColliderComponent>( glm::vec2{ 40.f, 20.f }, glm::vec2{ 40.f, 185.f } );
    object.add_component<engine::BoxColliderComponent>( glm::vec2{ 360.f, 20.f }, glm::vec2{ 140.f, 185.f } );
    object.add_component<engine::BoxColliderComponent>( glm::vec2{ 40.f, 20.f }, glm::vec2{ 560.f, 185.f } );
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
    if ( !fs::exists( dataLocation ) )
    {
        dataLocation = "../resources/";
    }
#endif
    engine::Minigin engine{ dataLocation };
    engine.run( load );
    return 0;
}
