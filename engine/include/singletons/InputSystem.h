#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include "Singleton.h"
#include "bindings/BindingTypes.h"
#include "bindings/InputMappingContext.h"

// +--------------------------------+
// | GLM HEADERS					|
// +--------------------------------+
#include <glm.hpp>

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <memory>
#include <unordered_set>


namespace engine
{
    class InputSystem final : public Singleton<InputSystem>
    {
    public:
        bool process_input( );

        engine::InputMappingContext& get_input_mapping_context( );

    private:
        bool request_quit_{ false };

        engine::InputMappingContext input_mapping_context_{};

        std::unordered_set<binding::key_t> keys_pressed_this_frame_{};
        std::unordered_set<binding::btn_t> buttons_pressed_this_frame_{};

        void poll_sdl_events( );

        void press_key( binding::key_t key );
        void release_key( binding::key_t key );

        void press_button( binding::btn_t button );
        void release_button( binding::btn_t button );

        void dispatch_pressed( );

    };

    extern InputSystem& INPUT_SYSTEM;

}

#endif // INPUTSYSTEM_H
