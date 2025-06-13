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

#include <framework/Scene.h>

#include <singleton/GameInstance.h>
#include <singleton/ResourceManager.h>
#include <singleton/ScenePool.h>

#include <registration/audio.h>
#include <registration/input.h>
#include <registration/object_initializers.h>

#include <controller/CharacterController.h>
#include <controller/DebugController.h>
#include <component/JoinMenuComponent.h>
#include <framework/component/TextComponent.h>
#include <framework/component/TextureComponent.h>

#include <filesystem>
namespace fs = std::filesystem;


void load( )
{
    game::initialize_input( );
    game::initialize_audio( );

    engine::GAME_INSTANCE.set_gravity_coefficient( 400.f );

    const auto font = engine::RESOURCE_MANAGER.load_font( "fonts/pixelify.ttf", 36 );
    auto& scene     = engine::SCENE_POOL.create_scene( "demo" );
    engine::SCENE_POOL.select_scene( "demo" );

    auto& join = scene.create_object( );
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

    auto& versus = selection.create_child( );
    versus.add_component<engine::TextComponent>( "versus", font );
    versus.set_world_transform( engine::Transform::from_translation( { 370.f, 410.f } ) );

    join.add_component<game::JoinMenuComponent>( &selection, std::vector{ &player1, &player2 } );

    engine::GAME_INSTANCE.add_controller<game::DebugController>( );
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
