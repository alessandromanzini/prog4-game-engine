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
#include <framework/component/TextComponent.h>

#include <singleton/GameInstance.h>
#include <singleton/ResourceManager.h>
#include <singleton/ScenePool.h>

#include <registration/audio.h>
#include <registration/input.h>
#include <registration/object_initializers.h>

#include <controller/CharacterController.h>
#include <controller/DebugController.h>
#include <component/JoinMenuComponent.h>

#include <filesystem>
#include <framework/component/TextureComponent.h>
namespace fs = std::filesystem;


void load( )
{
    game::initialize_input( );
    game::initialize_audio( );

    engine::GAME_INSTANCE.set_gravity_coefficient( 400.f );

    const auto font = engine::RESOURCE_MANAGER.load_font( "fonts/pixelify.ttf", 36 );
    auto& scene     = engine::SCENE_POOL.create_scene( "Demo" );

    // Bub
    //auto& bub = scene.create_object( );
    //game::create_bub( bub, { 200.f, 375.f } );

    //auto& zenchan = scene.create_object( );
    //game::create_zenchan( zenchan, { 300.f, 375.f }, { &bub } );

    //auto& grid = scene.create_object( );
    //game::create_grid( grid );

    auto& join = scene.create_object( );
    join.add_component<engine::TextureComponent>( "maps/main_menu.png" );

    auto& player1 = join.create_child( );
    auto& player2 = join.create_child( );

    auto& selection = join.create_child( );

    auto& arcade = selection.create_child(  );
    arcade.add_component<engine::TextComponent>( "arcade", font );

    auto& versus = selection.create_child(  );
    versus.add_component<engine::TextComponent>( "versus", font );

    join.add_component<game::JoinMenuComponent>( &selection, std::vector{ &player1, &player2 } );

    //engine::GAME_INSTANCE.add_controller<game::DebugController>( );

    // auto& controller = engine::GAME_INSTANCE.add_controller<game::CharacterController>( );
    //controller.possess( &bub );
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
