#include "InputUtils.h"

#include "singletons/InputSystem.h"
#include "bindings/InputMappingContext.h"

using namespace engine;
using namespace engine::binding;

namespace game
{

	void initialize_input( )
	{
		auto& imc{ INPUT_SYSTEM.get_input_mapping_context( ) };

		// trigger_bitset_t bitset = bitmask_cast( TriggerEvent::Pressed );
		imc.register_input_action( UID( IA::Move ), SDL_KeyCode::SDLK_a, InputAction<bool>{}, bitset_cast( TriggerEvent::PRESSED ) );
	}

}