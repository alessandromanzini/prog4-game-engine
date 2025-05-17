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
#include <framework/component/physics/RigidBodyComponent.h>

#include <framework/behaviour/fsm/transitions.h>
#include <framework/resource/texture/Sprite2D.h>

#include <singleton/GameInstance.h>
#include <singleton/ResourceManager.h>
#include <singleton/ScenePool.h>
#include <singleton/ServiceLocator.h>

#include <registration/audio.h>
#include <registration/input.h>

#include <component/BubStateComponent.h>

#include <controller/CharacterController.h>
#include <controller/DebugController.h>

#include <filesystem>
#include <framework/component/physics/BoxColliderComponent.h>
namespace fs = std::filesystem;
namespace cnd = engine::fsm::condition;


void create_bub( engine::GameObject& object )
{
    object.set_world_transform( engine::Transform::from_translation( { 200.f, 375.f } ) );

    auto& rb = object.add_component<engine::RigidBodyComponent>( );
    rb.set_simulate_physics( false );

    object.add_component<engine::BoxColliderComponent>( glm::vec2{ 50.f, 50.f } );

    object.add_component<game::BubStateComponent>( );

    object.add_component<engine::AudioComponent>( "victory.wav", engine::sound::SoundType::SOUND_EFFECT,
                                                  engine::UID( game::AudioCue::GENERAL ) );
    object.add_component<engine::AudioComponent>( "pacdeath.wav", engine::sound::SoundType::SOUND_EFFECT,
                                                  engine::UID( game::AudioCue::GENERAL ) ).set_playback_on_deletion( );
}

void log( engine::ColliderComponent& coll1, engine::ColliderComponent& coll2 )
{
    printf( "Collision START between %p and %p\n", static_cast<void*>( &coll1 ), static_cast<void*>( &coll2 ) );
}

void log2( engine::ColliderComponent& coll1, engine::ColliderComponent& coll2 )
{
    printf( "Collision OVER between %p and %p\n", static_cast<void*>( &coll1 ), static_cast<void*>( &coll2 ) );
}

void load( )
{
    game::initialize_input( );
    game::initialize_audio( );

    engine::GAME_INSTANCE.set_gravity_coefficient( 100.f );

    const auto font = engine::RESOURCE_MANAGER.load_font( "fonts/Lingua.otf", 36 );
    auto& scene     = engine::SCENE_POOL.create_scene( "Demo" );

    // Bub
    auto& bub = scene.create_object( );
    create_bub( bub );

    auto& block = scene.create_object( );
    auto& coll = block.add_component<engine::BoxColliderComponent>( glm::vec2{ 100.f, 1500.f } );
    coll.on_begin_overlap.bind( log );
    coll.on_end_overlap.bind( log2 );

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
    engine::Minigin engine( dataLocation );
    engine.run( load );
    return 0;
}
