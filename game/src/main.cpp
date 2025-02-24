#include <SDL.h>

#if _DEBUG
	// ReSharper disable once CppUnusedIncludeDirective
	#if __has_include(<vld.h>)
		#include <vld.h>
	#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "TextureComponent.h"
#include "TextComponent.h"
#include "RotatingComponent.h"
#include "FPS.h"
#include "Scene.h"

#include <filesystem>
namespace fs = std::filesystem;

void load( )
{
	auto pFont = engine::ResourceManager::get_instance( ).load_font( "Lingua.otf", 36 );
	auto& scene = engine::SceneManager::get_instance( ).create_scene( "Demo" );

	// bg GameObject
	auto bgGo = std::make_shared<engine::GameObject>( );
	bgGo->add_component<engine::TextureComponent>( "background.tga" );

	// DAE logo component
	auto logoGo = std::make_shared<engine::GameObject>( );
	logoGo->set_world_transform( glm::vec2{ 216.f, 180.f } );
	logoGo->add_component<engine::TextureComponent>( "logo.tga" );

	// Programming 4 Assignment text component
	auto textGo = std::make_shared<engine::GameObject>( );
	textGo->set_world_transform( glm::vec2{ 80.f, 20.f } );
	textGo->add_component<engine::TextComponent>( "Programming 4 Assignment", pFont );

	// FPS GameObject
	auto fpsGo = std::make_shared<engine::GameObject>( );
	fpsGo->set_world_transform( glm::vec2{ 20.f, 425.f } );
	fpsGo->add_component<engine::TextComponent>( "", pFont );
	fpsGo->add_component<engine::FPS>( );

	// Rotating Pacman GameObject
	auto rotatorGo = std::make_shared<engine::GameObject>( );
	rotatorGo->set_world_transform( glm::vec2{ 125.f, 315.f } );

	auto pacmanGo = std::make_shared<engine::GameObject>( );
	pacmanGo->add_component<engine::TextureComponent>( "pacman.png" );
	pacmanGo->add_component<engine::RotatingComponent>( glm::radians( 180.f ) );
	pacmanGo->set_parent( rotatorGo.get( ) );
	pacmanGo->set_local_transform( glm::vec2{ 15.f, 0.f } );

	auto mspacmanGo = std::make_shared<engine::GameObject>( );
	mspacmanGo->add_component<engine::TextureComponent>( "ms-pacman.png" );
	mspacmanGo->add_component<engine::RotatingComponent>( glm::radians( -360.f ) );
	mspacmanGo->set_parent( pacmanGo.get( ) );
	mspacmanGo->set_local_transform( glm::vec2{ 50.f, 0.f } );

	scene.add( bgGo );
	scene.add( logoGo );
	scene.add( textGo );
	scene.add( fpsGo );
	scene.add( rotatorGo );
	scene.add( pacmanGo );
	scene.add( mspacmanGo );
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
