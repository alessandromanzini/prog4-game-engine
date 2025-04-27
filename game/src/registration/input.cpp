#include <registration/input.h>

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

        imc.register_input_action( InputAction{ UID( IA::MOVE ), Modifier::SWIZZLE, Modifier::NEGATE }, SDLK_w );
        imc.register_input_action( InputAction{ UID( IA::MOVE ), Modifier::SWIZZLE }, SDLK_s );
        imc.register_input_action( InputAction{ UID( IA::MOVE ), Modifier::NEGATE }, SDLK_a );
        imc.register_input_action( InputAction{ UID( IA::MOVE ) }, SDLK_d );

        imc.register_input_action( UID( IA::MOVE ), SDL_CONTROLLER_BUTTON_A );
        imc.register_input_action( InputAction{ UID( IA::MOVE ), Modifier::NEGATE }, SDL_CONTROLLER_BUTTON_B );

        imc.register_input_action( UID( IA::PLAY_SOUND ), SDLK_f );

        imc.register_input_action( UID( IA::VOLUME_UP ), SDLK_UP );
        imc.register_input_action( UID( IA::VOLUME_DOWN ), SDLK_DOWN );

        imc.register_input_action( UID( IA::KILL ), SDLK_k );

        imc.register_input_action( UID( IA::QUIT ), SDLK_ESCAPE );
    }

}
