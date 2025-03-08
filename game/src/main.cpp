#include <SDL.h>

#if _DEBUG
	// ReSharper disable once CppUnusedIncludeDirective
	#if __has_include(<vld.h>)
		#include <vld.h>
	#endif
#endif

// +--------------------------------+
// | Project Headers				|
// +--------------------------------+
#include "singletons/ScenePool.h"
#include "singletons/UIController.h"
#include "singletons/ResourceManager.h"
#include "components/TextureComponent.h"
#include "components/TextComponent.h"
#include "components/FPS.h"
#include "components/RotatingComponent.h"
#include "components/InputDisplayUIComponent.h"
#include "components/DemoUIComponent.h"
#include "Minigin.h"
#include "Scene.h"
#include "GameObject.h"

// +--------------------------------+
// | Standard Headers				|
// +--------------------------------+
#include <filesystem>
namespace fs = std::filesystem;

void load( )
{
	auto pFont = engine::RESOURCE_MANAGER.load_font( "Lingua.otf", 36 );
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
	textGo->add_component<engine::TextComponent>( "Programming 4 Assignment", pFont );
	
	// FPS GameObject
	auto fpsGo = scene.create_object( );
	fpsGo->set_world_transform( glm::vec2{ 20.f, 425.f } );
	fpsGo->add_component<engine::TextComponent>( "", pFont );
	fpsGo->add_component<engine::FPS>( );

	// Rotating PacMan GameObject
	auto rotatorGo = scene.create_object( );
	rotatorGo->set_world_transform( glm::vec2{ 125.f, 315.f } );

	auto pacmanGo = scene.create_object( );
	pacmanGo->add_component<engine::TextureComponent>( "pacman.png" );
	pacmanGo->add_component<game::RotatingComponent>( glm::radians( 180.f ) );
	pacmanGo->set_parent( rotatorGo );
	pacmanGo->set_local_transform( glm::vec2{ 15.f, 0.f } );

	auto mspacmanGo = scene.create_object( );
	mspacmanGo->add_component<engine::TextureComponent>( "ms-pacman.png" );
	mspacmanGo->add_component<game::RotatingComponent>( glm::radians( -360.f ) );
	mspacmanGo->set_parent( pacmanGo );
	mspacmanGo->set_local_transform( glm::vec2{ 50.f, 0.f } );

	engine::UI_CONTROLLER.add_ui_component<game::DemoUIComponent>( );
	engine::UI_CONTROLLER.add_ui_component<engine::InputDisplayUIComponent>( );
}

int main( int, char* [] )
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
