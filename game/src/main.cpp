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
#include <components/FPS.h>
#include <components/MoveComponent.h>
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

#include <controllers/TestController.h>

// +--------------------------------+
// | Standard Headers				|
// +--------------------------------+
#include <filesystem>
namespace fs = std::filesystem;


void boilerplate_objects( engine::Scene& scene, const std::shared_ptr<engine::Font>& font )
{
    // bg GameObject
    const auto bgGo = scene.create_object( );
    bgGo->add_component<engine::TextureComponent>( "background.tga" );

    // DAE logo component
    const auto logoGo = scene.create_object( );
    logoGo->set_world_transform( engine::Transform::from_translation( { 216.f, 180.f } ) );
    logoGo->add_component<engine::TextureComponent>( "logo.tga" );

    // Programming 4 Assignment text component
    const auto textGo = scene.create_object( );
    textGo->set_world_transform( engine::Transform::from_translation( { 80.f, 20.f } ) );
    textGo->add_component<engine::TextComponent>( "Programming 4 Assignment", font );

    // FPS GameObject
    const auto fpsGo = scene.create_object( );
    fpsGo->set_world_transform( engine::Transform::from_translation( { 20.f, 425.f } ) );
    fpsGo->add_component<engine::TextComponent>( "", font );
    fpsGo->add_component<engine::FPS>( );
}


void load( )
{
    game::initialize_input( );

    const auto font   = engine::RESOURCE_MANAGER.load_font( "Lingua.otf", 36 );
    auto& scene = engine::SCENE_POOL.create_scene( "Demo" );

    boilerplate_objects( scene, font );

    // PacMan
    const auto pacmanGo = scene.create_object( );
    pacmanGo->add_component<engine::TextureComponent>( "pacman.png" );
    pacmanGo->set_world_transform( engine::Transform::from_translation( { 200.f, 375.f } ) );
    pacmanGo->add_component<game::MoveComponent>( 100.f );

    // Ms PacMan
    // const auto mspacmanGo = scene.create_object( );
    // mspacmanGo->add_component<engine::TextureComponent>( "ms-pacman.png" );
    // mspacmanGo->set_world_transform( glm::vec2{ 200.f, 325.f } );

    auto* controller = new game::TestController();
    controller->possess( pacmanGo );

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
