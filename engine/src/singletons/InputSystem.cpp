#include "singletons/InputSystem.h"

// +--------------------------------+
// | SDL Headers					|
// +--------------------------------+
#include <SDL.h>

using namespace engine;

InputSystem& engine::INPUT_SYSTEM = InputSystem::get_instance( );

bool InputSystem::process_input()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {
			
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			
		}
		// etc...
	}

	return true;
}
