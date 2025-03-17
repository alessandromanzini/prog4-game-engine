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

		imc.register_input_action( UID( IA::KATTACK ), key_t::SDLK_c, InputAction<bool>{}, bitset_cast( TriggerEvent::TRIGGERED ) );
		imc.register_input_action( UID( IA::GATTACK ), btn_t::SDL_CONTROLLER_BUTTON_X, InputAction<bool>{}, bitset_cast( TriggerEvent::TRIGGERED ) );

		imc.register_input_action( UID( IA::KSCORE_UP ), key_t::SDLK_z, InputAction<bool>{}, bitset_cast( TriggerEvent::TRIGGERED ) );
		imc.register_input_action( UID( IA::GSCORE_UP ), btn_t::SDL_CONTROLLER_BUTTON_A, InputAction<bool>{}, bitset_cast( TriggerEvent::TRIGGERED ) );

		imc.register_input_action( UID( IA::KSCORE_GREAT_UP ), key_t::SDLK_x, InputAction<bool>{}, bitset_cast( TriggerEvent::TRIGGERED ) );
		imc.register_input_action( UID( IA::GSCORE_GREAT_UP ), btn_t::SDL_CONTROLLER_BUTTON_B, InputAction<bool>{}, bitset_cast( TriggerEvent::TRIGGERED ) );
	}

}