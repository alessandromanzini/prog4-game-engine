#ifndef INPUTMAPPINGCONTEXT_H
#define INPUTMAPPINGCONTEXT_H

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include "bindings/BindingTypes.h"
#include "bindings/DeviceContext.h"
#include "bindings/InputAction.h"
#include "framework/UID.h"

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <list>
#include <memory>
#include <unordered_map>
#include <vector>


namespace engine
{
    class Command;
    class InputMappingContext final
    {
    public:
        // Registers an input action with the given key, input action, and triggers as a keyboard/gamepad binding.
        void register_input_action( UID uid, binding::key_t key, input_action_variant_t inputAction,
                                    const binding::trigger_bitset_t& triggers );
        void register_input_action( UID uid, binding::btn_t button, input_action_variant_t inputAction,
                                    const binding::trigger_bitset_t& triggers );

        void bind_input_action( UID uid, Command* command );
        void unbind_input_action( UID uid, Command* command );

        // Signals the input action with the given value and trigger as a keyboard/gamepad event.
        void signal( binding::key_t key, bool value, binding::TriggerEvent trigger );
        void signal( binding::btn_t button, bool value, binding::TriggerEvent trigger );

        // Dispatches the merged signaled events to the corresponding commands.
        void dispatch( );

    private:
        // Device contexts for action bindings
        DeviceContext keyboard_context_{};
        DeviceContext gamepad_context_{};

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

        // Merges the signaled inputs of same uid as a single value for dispatching.
        void signal( UID uid, bool value, binding::TriggerEvent trigger );

        [[nodiscard]] bool is_input_action_registered_for_any( UID uid ) const;
        [[nodiscard]] bool is_input_action_registered_for_key( UID uid, binding::key_t key ) const;
        [[nodiscard]] bool is_input_action_registered_for_button( UID uid, binding::btn_t button ) const;

    };

}


#endif // !INPUTMAPPINGCONTEXT_H
