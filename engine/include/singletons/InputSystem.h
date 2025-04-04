#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include "bindings/binding_types.h"
#include "bindings/InputMappingContext.h"
#include "singletons/Singleton.h"

// +--------------------------------+
// | GLM HEADERS					|
// +--------------------------------+
#include <glm.hpp>

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <binding_device.h>
#include <unordered_set>


namespace engine
{
    // TODO: Pimpl InputSystem to hide SDL dependencies. Use service locator pattern
    class InputSystem final : public Singleton<InputSystem>
    {
    public:
        bool process_input( );

        InputMappingContext& get_input_mapping_context( );

    private:
        bool request_quit_{ false };

        InputMappingContext input_mapping_context_{};

        std::unordered_set<binding::key_t> keys_pressed_this_frame_{};
        std::unordered_set<binding::btn_t> buttons_pressed_this_frame_{};

        void poll( );

        void forward_code_to_contexts( binding::UniformBindingCode code, binding::TriggerEvent trigger,
                                       binding::DeviceInfo deviceInfo );

        void trigger( binding::UniformBindingCode code, binding::DeviceInfo deviceInfo );
        void release( binding::UniformBindingCode code, binding::DeviceInfo deviceInfo );
        void press( );

    };

    extern InputSystem& INPUT_SYSTEM;

}

#endif //!INPUTSYSTEM_H
