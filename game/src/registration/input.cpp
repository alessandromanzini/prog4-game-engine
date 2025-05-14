#include <registration/input.h>

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <singleton/InputSystem.h>

using namespace engine;
using namespace engine::binding;


namespace game
{

    void initialize_input( )
    {
        auto& imc{ INPUT_SYSTEM.get_input_mapping_context( ) };

        imc.register_input_action( InputAction{ UID( IA::MOVE ), Modifier::SWIZZLE, Modifier::NEGATE }, SDLK_w );
        imc.register_input_action( InputAction{ UID( IA::MOVE ), Modifier::SWIZZLE }, SDLK_s );
        imc.register_input_action( InputAction{ UID( IA::MOVE ), Modifier::NEGATE }, SDLK_a );
        imc.register_input_action( InputAction{ UID( IA::MOVE ) }, SDLK_d );

        imc.register_input_action( InputAction{ UID( IA::MOVE ), Modifier::SWIZZLE, Modifier::NEGATE }, SDL_CONTROLLER_BUTTON_DPAD_UP );
        imc.register_input_action( InputAction{ UID( IA::MOVE ), Modifier::SWIZZLE }, SDL_CONTROLLER_BUTTON_DPAD_DOWN );
        imc.register_input_action( InputAction{ UID( IA::MOVE ), Modifier::NEGATE }, SDL_CONTROLLER_BUTTON_DPAD_LEFT );
        imc.register_input_action( InputAction{ UID( IA::MOVE ) }, SDL_CONTROLLER_BUTTON_DPAD_RIGHT );

        imc.register_input_action( UID( IA::JUMP ), SDL_CONTROLLER_BUTTON_A );

        imc.register_input_action( UID( IA::PLAY_SOUND ), SDLK_f );

        imc.register_input_action( UID( IA::VOLUME_UP ), SDLK_UP );
        imc.register_input_action( UID( IA::VOLUME_DOWN ), SDLK_DOWN );

        imc.register_input_action( UID( IA::KILL ), SDLK_k );

        imc.register_input_action( UID( IA::QUIT ), SDLK_ESCAPE );
        imc.register_input_action( UID( IA::QUIT ), SDL_CONTROLLER_BUTTON_START );
    }

}
