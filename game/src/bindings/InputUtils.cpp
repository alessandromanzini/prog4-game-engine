#include "InputUtils.h"

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include "bindings/InputMappingContext.h"
#include "singletons/InputSystem.h"


using namespace engine;
using namespace engine::binding;

namespace game
{

	void initialize_input( )
	{
		auto& imc{ INPUT_SYSTEM.get_input_mapping_context( ) };

		imc.register_input_action( UID( IA::KATTACK ), binding::key_t::SDLK_c, InputAction<bool>{}, binding::bitset_cast( TriggerEvent::TRIGGERED ) );
		imc.register_input_action( UID( IA::GATTACK ), binding::btn_t::SDL_CONTROLLER_BUTTON_X, InputAction<bool>{}, bitset_cast( TriggerEvent::TRIGGERED ) );

		imc.register_input_action( UID( IA::KSCORE_UP ), binding::key_t::SDLK_z, InputAction<bool>{}, bitset_cast( TriggerEvent::TRIGGERED ) );
		imc.register_input_action( UID( IA::GSCORE_UP ), btn_t::SDL_CONTROLLER_BUTTON_A, InputAction<bool>{}, bitset_cast( TriggerEvent::TRIGGERED ) );

		imc.register_input_action( UID( IA::KSCORE_GREAT_UP ), binding::key_t::SDLK_x, InputAction<bool>{}, bitset_cast( TriggerEvent::TRIGGERED ) );
		imc.register_input_action( UID( IA::GSCORE_GREAT_UP ), binding::btn_t::SDL_CONTROLLER_BUTTON_B, InputAction<bool>{}, bitset_cast( TriggerEvent::TRIGGERED ) );
	}

}