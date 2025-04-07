#include <core/Minigin.h>

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <singletons/GameTime.h>
#include <singletons/InputSystem.h>
#include <singletons/Renderer.h>
#include <singletons/ResourceManager.h>
#include <singletons/ScenePool.h>
#include <singletons/UIController.h>

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <iostream>
#include <stdexcept>

#if WIN32
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#endif

// +--------------------------------+
// | SDL HEADERS					|
// +--------------------------------+
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <thread>


namespace engine
{
    SDL_Window* g_window_ptr{};


    void LogSDLVersion( const std::string& message, const SDL_version& v )
    {
#if WIN32
		std::stringstream ss;
		ss << message << (int) v.major << "." << (int) v.minor << "." << (int) v.patch << "\n";
		OutputDebugString( ss.str( ).c_str( ) );
#else
        std::cout << message << static_cast<int>( v.major ) << "." << static_cast<int>( v.minor ) << "." << static_cast<
            int>( v.patch ) << "\n";
#endif
    }

#ifdef __EMSCRIPTEN__
#include <emscripten.h>

	void LoopCallback( void* arg )
	{
		static_cast<dae::Minigin*>( arg )->RunOneFrame( );
	}
#endif

    // Why bother with this? Because sometimes students have a different SDL version installed on their pc.
    // That is not a problem unless for some reason the dll's from this project are not copied next to the exe.
    // These entries in the debug output help to identify that issue.
    void print_sdl_version( )
    {
        SDL_version version{};
        SDL_VERSION( &version );
        LogSDLVersion( "Compiled SDL version ", version );

        SDL_GetVersion( &version );
        LogSDLVersion( "Linked SDL version ", version );

        SDL_IMAGE_VERSION( &version );
        LogSDLVersion( "Compiled SDL_image version ", version );

        version = *IMG_Linked_Version( );
        LogSDLVersion( "Linked SDL_image version ", version );

        SDL_TTF_VERSION( &version );
        LogSDLVersion( "Compiled SDL_ttf version ", version );

        version = *TTF_Linked_Version( );
        LogSDLVersion( "Linked SDL_ttf version ", version );
    }


    Minigin::Minigin( const std::filesystem::path& dataPath )
    {
        print_sdl_version( );

        if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER ) != 0 )
        {
            throw std::runtime_error( std::string( "SDL_Init Error: " ) + SDL_GetError( ) );
        }

        g_window_ptr = SDL_CreateWindow(
            "Bubble Bobble - 2GD10 Manzini Alessandro",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            640,
            480,
            SDL_WINDOW_OPENGL
        );
        if ( g_window_ptr == nullptr )
        {
            throw std::runtime_error( std::string( "SDL_CreateWindow Error: " ) + SDL_GetError( ) );
        }

        // Initialize singletons
        RENDERER.init( g_window_ptr );
        UI_CONTROLLER.init( );
        RESOURCE_MANAGER.init( dataPath );
    }


    Minigin::~Minigin( ) noexcept
    {
        // Destroy singletons
        RENDERER.destroy( );
        UI_CONTROLLER.destroy( );

        SDL_DestroyWindow( g_window_ptr );
        g_window_ptr = nullptr;
        SDL_Quit( );
    }


    void Minigin::run( const std::function<void( )>& load )
    {
        load( );
        GAME_TIME.reset( );
#ifndef __EMSCRIPTEN__
        while ( !quit_ )
            run_one_frame( );
#else
		emscripten_set_main_loop_arg( &LoopCallback, this, 0, true );
#endif
    }


    void Minigin::run_one_frame( )
    {
        // +--------------------------------+
        // | TIME CALCULATIONS ( ticking )	|
        // +--------------------------------+
        GAME_TIME.tick( );

        // +--------------------------------+
        // | GAME LOOP						|
        // +--------------------------------+
        quit_ = !INPUT_SYSTEM.process_input( );
        while ( GAME_TIME.get_required_fixed_update( ) )
        {
            // Call the fixed update and tick the lag time
            GAME_TIME.set_timing_type( TimingType::FIXED_DELTA_TIME );
            SCENE_POOL.fixed_update( );
            GAME_TIME.fixed_tick( );
        }
        GAME_TIME.set_timing_type( TimingType::DELTA_TIME );
        SCENE_POOL.update( );
        UI_CONTROLLER.update( );
        RENDERER.render( );

        // +--------------------------------+
        // | DESTROYED OBJECTS DELETION		|
        // +--------------------------------+
        SCENE_POOL.cleanup( );
        UI_CONTROLLER.cleanup( );

        // +--------------------------------+
        // | SLEEPING						|
        // +--------------------------------+
        const auto sleepTime = GAME_TIME.get_sleep_time( );
        std::this_thread::sleep_for( sleepTime );
    }
}
