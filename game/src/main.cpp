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
#include <framework/component/RigidBodyComponent.h>
#include <framework/component/TextComponent.h>

#include <framework/behaviour/fsm/transitions.h>
#include <framework/resource/texture/Sprite2D.h>

#include <singleton/ResourceManager.h>
#include <singleton/ScenePool.h>
#include <singleton/ServiceLocator.h>
#include <singleton/UIController.h>

#include <registration/audio.h>
#include <registration/input.h>

#include <component/BubStateComponent.h>
#include <ui_component/AudioTestUIComponent.h>

#include <controller/TestController.h>

// +--------------------------------+
// | Standard Headers				|
// +--------------------------------+
#include <filesystem>
namespace fs = std::filesystem;
namespace cnd = engine::fsm::condition;


void create_bub( engine::GameObject& object )
{
    object.set_world_transform( engine::Transform::from_translation( { 200.f, 375.f } ) );

    object.add_component<engine::RigidBodyComponent>( );

    // auto& selector = object.add_component<game::BubStateComponent>( );
    // selector.add_sprite( engine::UID( "idle" ),
    //                      engine::Sprite2D{
    //                          "characters/bub/bub_idle_1x8.png",
    //                          static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 8u ), 0.2f, 2.f
    //                      } );
    // selector.add_sprite( engine::UID( "walk" ),
    //                      engine::Sprite2D{
    //                          "characters/bub/bub_walk_1x6.png",
    //                          static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 6u ),
    //                          0.2f, 2.f, { 0.f, -3.f }
    //                      } );
    // selector.add_sprite( engine::UID( "jump" ),
    //                      engine::Sprite2D{
    //                          "characters/bub/bub_jump_1x2.png",
    //                          static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 2u ),
    //                          0.2f, 2.f, {}, false
    //                      } );
    // selector.add_sprite( engine::UID( "fall" ),
    //                      engine::Sprite2D{
    //                          "characters/bub/bub_fall_1x1.png",
    //                          static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 1u ),
    //                          0.2f, 2.f
    //                      } );
    // selector.make_transition<cnd::IsMovingHorizontallyCondition>( engine::UID( "idle" ), engine::UID( "walk" ) );
    // selector.make_transition<cnd::IsIdleCondition>( engine::UID( "walk" ), engine::UID( "idle" ) );
    //
    // selector.make_transition<cnd::IsNotGroundedCondition>( engine::UID( "walk" ), engine::UID( "jump" ) );
    // selector.make_transition<cnd::IsNotGroundedCondition>( engine::UID( "idle" ), engine::UID( "jump" ) );
    //
    // selector.make_transition<cnd::IsFallingCondition>( engine::UID( "jump" ), engine::UID( "fall" ) );
    //
    // selector.make_transition<cnd::IsGroundedIdleCondition>( engine::UID( "fall" ), engine::UID( "idle" ) );
    // selector.make_transition<cnd::IsGroundedMovingCondition>( engine::UID( "fall" ), engine::UID( "walk" ) );

    object.add_component<engine::AudioComponent>( "victory.wav", engine::sound::SoundType::SOUND_EFFECT,
                                                  engine::UID( game::AudioCue::GENERAL ) );
    object.add_component<engine::AudioComponent>( "pacdeath.wav", engine::sound::SoundType::SOUND_EFFECT,
                                                  engine::UID( game::AudioCue::GENERAL ) ).set_playback_on_deletion( );
}


void load( )
{
    game::initialize_input( );
    game::initialize_audio( );

    const auto font = engine::RESOURCE_MANAGER.load_font( "fonts/Lingua.otf", 36 );
    auto& scene     = engine::SCENE_POOL.create_scene( "Demo" );

    // Bub
    auto& bub = scene.create_object( );
    create_bub( bub );

    engine::SERVICE_LOCATOR.tempPlayerController = std::make_unique<game::TestController>( );
    engine::SERVICE_LOCATOR.tempPlayerController->possess( &bub );

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
