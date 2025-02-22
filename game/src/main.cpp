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
	logoGo->get_transform( ).global( ).set_position( glm::vec2{ 216.f, 180.f } );
	logoGo->add_component<engine::TextureComponent>( "logo.tga" );

	// Programming 4 Assignment text component
	auto textGo = std::make_shared<engine::GameObject>( );
	textGo->get_transform( ).local( ).set_position( glm::vec2{ 80.f, 20.f } );
	textGo->add_component<engine::TextComponent>( "Programming 4 Assignment", pFont );

	// FPS GameObject
	auto fpsGo = std::make_shared<engine::GameObject>( );
	fpsGo->get_transform( ).global( ).set_position( glm::vec2{ 20.f, 425.f } );
	fpsGo->add_component<engine::TextComponent>( "", pFont );
	fpsGo->add_component<engine::FPS>( );

	scene.add( bgGo );
	scene.add( logoGo );
	scene.add( textGo );
	scene.add( fpsGo );
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
