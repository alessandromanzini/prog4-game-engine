#include <InputUtils.h>

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <singletons/InputSystem.h>


using namespace engine;
using namespace engine::binding;

namespace game
{

    void initialize_input( )
    {
        auto& imc{ INPUT_SYSTEM.get_input_mapping_context( ) };

        imc.register_input_action( InputAction{ UID( IA::TEST ), Modifier::SWIZZLE, Modifier::NEGATE }, SDLK_w );
        imc.register_input_action( InputAction{ UID( IA::TEST ), Modifier::SWIZZLE }, SDLK_s );
        imc.register_input_action( InputAction{ UID( IA::TEST ), Modifier::NEGATE }, SDLK_a );
        imc.register_input_action( InputAction{ UID( IA::TEST ) }, SDLK_d );

        imc.register_input_action( UID( IA::TEST ), SDL_CONTROLLER_BUTTON_A );
        imc.register_input_action( InputAction{ UID( IA::TEST ), Modifier::NEGATE }, SDL_CONTROLLER_BUTTON_B );
    }

}
