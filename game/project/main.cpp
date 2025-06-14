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
#include <singleton/ScenePool.h>

#include <registration/audio.h>
#include <registration/input.h>
#include <registration/object_initializers.h>

#include <controller/DebugController.h>

#include <filesystem>
namespace fs = std::filesystem;


void load( )
{
    game::initialize_input( );
    game::initialize_audio( );

    engine::GAME_INSTANCE.set_gravity_coefficient( 400.f );

    auto& scene     = engine::SCENE_POOL.create_scene( "main" );
    engine::SCENE_POOL.select_scene( "main" );

    auto& join = scene.create_object( );
    game::create_join_menu( join );

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
