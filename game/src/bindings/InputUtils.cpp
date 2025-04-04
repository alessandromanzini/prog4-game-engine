#include "InputUtils.h"

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include "singletons/InputSystem.h"


using namespace engine;
using namespace engine::binding;

namespace game
{

	void initialize_input( )
	{
		auto& imc{ INPUT_SYSTEM.get_input_mapping_context( ) };

		imc.register_input_action( UID( IA::TEST ), binding::key_t::SDLK_w );
	}

}