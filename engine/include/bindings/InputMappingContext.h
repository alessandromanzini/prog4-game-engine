#ifndef INPUTMAPPINGCONTEXT_H
#define INPUTMAPPINGCONTEXT_H

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include "bindings/BindingTypes.h"
#include "bindings/InputAction.h"
#include "framework/UID.h"

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <list>
#include <unordered_map>
#include <vector>


namespace engine
{
    class Command;
    class InputMappingContext final
    {
    public:
        void register_input_action( UID uid, binding::key_t key, input_action_variant_t inputAction,
                                    const binding::trigger_bitset_t& triggers );
        void register_input_action( UID uid, binding::btn_t button, input_action_variant_t inputAction,
                                    const binding::trigger_bitset_t& triggers );

        void bind_input_action( UID uid, Command* command );
        void unbind_input_action( UID uid, Command* command );

        void signal( binding::key_t key, bool value, binding::TriggerEvent trigger );
        void signal( binding::btn_t button, bool value, binding::TriggerEvent trigger );

        void dispatch( );

    private:
        // These maps hold the inputs mapped to the input actions containing binding information.
        std::unordered_map<binding::key_t, std::vector<InputActionBinding>> keyboard_actions_{};
        std::unordered_map<binding::btn_t, std::vector<InputActionBinding>> gamepad_actions_{};

        // This map holds the triggers for each action.
        std::unordered_map<UID, binding::trigger_bitset_t> action_triggers_{};

        // This queue holds the signaled events to be dispatched.
        std::vector<InputActionContext> signaled_triggered_inputs_{};
        std::vector<InputActionContext> signaled_pressed_inputs_{};

        // These maps hold the commands to be called when the corresponding input action is signaled.
        std::unordered_map<UID, std::list<Command*>> triggered_commands_{};
        std::unordered_map<UID, std::list<Command*>> pressed_commands_{};
        std::unordered_map<UID, std::list<Command*>> released_commands_{};

        void register_input_action( UID uid, input_action_variant_t inputAction,
                                    const binding::trigger_bitset_t& triggers,
                                    std::vector<InputActionBinding>& actions );
        void signal( UID uid, bool value, binding::TriggerEvent trigger );

        [[nodiscard]] bool is_input_action_registered( UID uid, const std::vector<InputActionBinding>& bindings ) const;
        [[nodiscard]] bool is_input_action_registered_for_any( UID uid ) const;
        [[nodiscard]] bool is_input_action_registered_for_key( UID uid, binding::key_t key ) const;
        [[nodiscard]] bool is_input_action_registered_for_button( UID uid, binding::btn_t button ) const;

    };

}

#endif // !INPUTMAPPINGCONTEXT_H
