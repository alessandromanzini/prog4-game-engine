#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
		#include <vld.h>
#endif
#endif

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <bindings/InputUtils.h>
#include <components/AttackComponent.h>
#include <components/FPS.h>
#include <components/HealthComponent.h>
#include <components/ScoreComponent.h>
#include <components/TextComponent.h>
#include <components/TextureComponent.h>
#include <core/Minigin.h>
#include <framework/GameObject.h>
#include <framework/Scene.h>
#include <singletons/ResourceManager.h>
#include <singletons/ScenePool.h>
#include <singletons/UIController.h>
#include <ui_components/DemoUIComponent.h>
#include <ui_components/InputDisplayUIComponent.h>
#include <ui_components/ObserverTestUIComponent.h>

#include <bindings/TestController.h>

// +--------------------------------+
// | Standard Headers				|
// +--------------------------------+
#include <filesystem>
namespace fs = std::filesystem;


void load( )
{
    game::initialize_input( );

    auto font   = engine::RESOURCE_MANAGER.load_font( "Lingua.otf", 36 );
    auto& scene = engine::SCENE_POOL.create_scene( "Demo" );

    // bg GameObject
    auto bgGo = scene.create_object( );
    bgGo->add_component<engine::TextureComponent>( "background.tga" );

    // DAE logo component
    auto logoGo = scene.create_object( );
    logoGo->set_world_transform( glm::vec2{ 216.f, 180.f } );
    logoGo->add_component<engine::TextureComponent>( "logo.tga" );

    // Programming 4 Assignment text component
    auto textGo = scene.create_object( );
    textGo->set_world_transform( glm::vec2{ 80.f, 20.f } );
    textGo->add_component<engine::TextComponent>( "Programming 4 Assignment", font );

    // FPS GameObject
    auto fpsGo = scene.create_object( );
    fpsGo->set_world_transform( glm::vec2{ 20.f, 425.f } );
    fpsGo->add_component<engine::TextComponent>( "", font );
    fpsGo->add_component<engine::FPS>( );

    // PacMan
    auto pacmanGo = scene.create_object( );
    pacmanGo->add_component<engine::TextureComponent>( "pacman.png" );
    pacmanGo->set_world_transform( glm::vec2{ 200.f, 375.f } );

    // Ms PacMan
    auto mspacmanGo = scene.create_object( );
    mspacmanGo->add_component<engine::TextureComponent>( "ms-pacman.png" );
    mspacmanGo->set_world_transform( glm::vec2{ 200.f, 325.f } );

    new game::TestController();

    // engine::UI_CONTROLLER.add_ui_component<game::DemoUIComponent>( );
    // engine::UI_CONTROLLER.add_ui_component<engine::InputDisplayUIComponent>( );
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
