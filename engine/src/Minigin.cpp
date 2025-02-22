#include "Minigin.h"

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "GameTime.h"

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <stdexcept>
#include <sstream>
#include <iostream>

#if WIN32
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#endif

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <thread>

using namespace engine;

SDL_Window* g_window_ptr{};

void LogSDLVersion(const std::string& message, const SDL_version& v)
{
#if WIN32
	std::stringstream ss;
	ss << message << (int)v.major << "." << (int)v.minor << "." << (int)v.patch << "\n";
	OutputDebugString(ss.str().c_str());
#else
	std::cout << message << (int)v.major << "." << (int)v.minor << "." << (int)v.patch << "\n";
#endif
}

#ifdef __EMSCRIPTEN__
#include "emscripten.h"

void LoopCallback(void* arg)
{
	static_cast<dae::Minigin*>(arg)->RunOneFrame();
}
#endif

// Why bother with this? Because sometimes students have a different SDL version installed on their pc.
// That is not a problem unless for some reason the dll's from this project are not copied next to the exe.
// These entries in the debug output help to identify that issue.
void PrintSDLVersion()
{
	SDL_version version{};
	SDL_VERSION(&version);
	LogSDLVersion("We compiled against SDL version ", version);

	SDL_GetVersion(&version);
	LogSDLVersion("We linked against SDL version ", version);

	SDL_IMAGE_VERSION(&version);
	LogSDLVersion("We compiled against SDL_image version ", version);

	version = *IMG_Linked_Version();
	LogSDLVersion("We linked against SDL_image version ", version);

	SDL_TTF_VERSION(&version)
	LogSDLVersion("We compiled against SDL_ttf version ", version);

	version = *TTF_Linked_Version();
	LogSDLVersion("We linked against SDL_ttf version ", version);
}

Minigin::Minigin(const std::filesystem::path &dataPath)
{
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	g_window_ptr = SDL_CreateWindow(
		"Prog4 Assignment - Update GameObjects - 2GD10 Manzini Alessandro",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if (g_window_ptr == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::get_instance().init(g_window_ptr);
	ResourceManager::get_instance().init(dataPath);
}

Minigin::~Minigin()
{
	Renderer::get_instance().destroy();
	SDL_DestroyWindow(g_window_ptr);
	g_window_ptr = nullptr;
	SDL_Quit();
}

void Minigin::run(const std::function<void()>& load)
{
	load();
	GameTime::get_instance( ).reset( );
#ifndef __EMSCRIPTEN__
	while (!quit_)
		run_one_frame();
#else
	emscripten_set_main_loop_arg(&LoopCallback, this, 0, true);
#endif
}

void Minigin::run_one_frame()
{
	// +--------------------------------+
	// | TIME CALCULATIONS ( ticking )	|
	// +--------------------------------+
	GameTime::get_instance( ).tick( );

	// +--------------------------------+
	// | GAME LOOP						|
	// +--------------------------------+
	quit_ = !InputManager::get_instance().process_input();
	while ( GameTime::get_instance( ).get_required_fixed_update( ) )
	{
		// Call the fixed update and tick the lag time
		SceneManager::get_instance( ).fixed_update( );
		GameTime::get_instance( ).fixed_tick( );
	}
	SceneManager::get_instance().update();
	Renderer::get_instance().render();

	// +--------------------------------+
	// | DESTROYED OBJECTS DELETION		|
	// +--------------------------------+
	SceneManager::get_instance( ).cleanup( );

	// +--------------------------------+
	// | SLEEPING						|
	// +--------------------------------+
	const auto sleepTime = GameTime::get_instance( ).get_sleep_time( );
	std::this_thread::sleep_for( sleepTime );
}
