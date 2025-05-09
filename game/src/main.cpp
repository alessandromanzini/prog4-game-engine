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

#include <registration/audio.h>
#include <registration/input.h>

#include <components/FPS.h>
#include <components/MoveComponent.h>
#include <framework/GameObject.h>
#include <framework/Scene.h>
#include <framework/components/AudioComponent.h>
#include <framework/components/SpriteComponent.h>
#include <framework/components/TextComponent.h>
#include <framework/components/TextureComponent.h>
#include <ui_components/AudioTestUIComponent.h>

#include <singletons/ResourceManager.h>
#include <singletons/ScenePool.h>
#include <singletons/ServiceLocator.h>
#include <singletons/UIController.h>

#include <controllers/TestController.h>

// +--------------------------------+
// | Standard Headers				|
// +--------------------------------+
#include <filesystem>
namespace fs = std::filesystem;


void load( )
{
    game::initialize_input( );
    game::initialize_audio( );

    const auto font = engine::RESOURCE_MANAGER.load_font( "Lingua.otf", 36 );
    auto& scene     = engine::SCENE_POOL.create_scene( "Demo" );

    // PacMan
    auto& pacmanGo = scene.create_object( );
    pacmanGo.add_component<engine::SpriteComponent>( "characters/bub/bub_idle_1x8.png", static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 8u ), 0.1f );
    pacmanGo.set_world_transform( engine::Transform::from_translation( { 200.f, 375.f } ) );
    pacmanGo.add_component<game::MoveComponent>( 100.f );
    pacmanGo.add_component<engine::AudioComponent>( "victory.wav", engine::sound::SoundType::SOUND_EFFECT,
                                                     engine::UID( game::AudioCue::GENERAL ) );
    pacmanGo.add_component<engine::AudioComponent>( "pacdeath.wav", engine::sound::SoundType::SOUND_EFFECT,
                                                     engine::UID( game::AudioCue::GENERAL ) ).set_playback_on_deletion(  );

    engine::SERVICE_LOCATOR.tempPlayerController = std::make_unique<game::TestController>( );
    engine::SERVICE_LOCATOR.tempPlayerController->possess( &pacmanGo );

    // engine::UI_CONTROLLER.add_ui_component<game::DemoUIComponent>( );
    // engine::UI_CONTROLLER.add_ui_component<engine::InputDisplayUIComponent>( );
    engine::UI_CONTROLLER.add_ui_component<game::AudioTestUIComponent>( );
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
