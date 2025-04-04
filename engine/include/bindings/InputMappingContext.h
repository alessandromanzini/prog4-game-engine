#ifndef INPUTMAPPINGCONTEXT_H
#define INPUTMAPPINGCONTEXT_H

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <bindings/binding_device.h>
#include <bindings/binding_structs.h>
#include <bindings/binding_types.h>
#include <bindings/binding_traits.h>
#include <framework/UID.h>

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <binding_controls.h>
#include <deque>
#include <list>
#include <unordered_map>


namespace engine
{
    class Command;
    class PlayerController;
    class InputMappingContext final
    {
    public:
        // Registers an input action as a UID bound to the given key code and modifiers.
        void register_input_action( const binding::InputAction& action, binding::UniformBindingCode code );
        void register_input_action( UID uid, binding::UniformBindingCode code );

        void register_device( PlayerController& controller, binding::DeviceInfo deviceInfo );
        void unregister_device( const PlayerController& controller );

        // Bind a member function to the input action of the given code. Command will be called once the input action is signaled.
        template <typename method_signature_t>
            requires std::derived_from<typename binding::method_traits<method_signature_t>::class_t, PlayerController>
        void bind_to_input_action( typename binding::method_traits<method_signature_t>::class_t& controller,
                                   UID uid,
                                   method_signature_t command,
                                   const binding::trigger_bitset_t& triggers = {
                                       seq_mask_cast( binding::TriggerEvent::TRIGGERED )
                                   } );

        void unbind_from_input_action( PlayerController& controller, UID uid );

        // Signals the input action bound to the given code and trigger for the correct device.
        void signal( binding::UniformBindingCode code, binding::TriggerEvent trigger, binding::DeviceInfo deviceInfo );

        // Dispatches the merged signaled events to the corresponding commands.
        void dispatch( );

    private:
        std::unordered_map<binding::UniformBindingCode, std::vector<binding::InputAction>,
            binding::UniformBindingCodeHasher> action_binds_{};
        std::list<binding::DeviceContext> device_contexts_{};

        // This queue holds the signaled events to be dispatched.
        std::deque<binding::InputSnapshot> signaled_inputs_queue_{};

        using optional_device_it = std::optional<decltype(device_contexts_)::iterator>;
        [[nodiscard]] optional_device_it find_device_context( const PlayerController& controller );

        void bind_to_input_action_impl( const PlayerController& controller, UID uid,
                                        binding::input_command_variant_t&& command,
                                        const binding::trigger_bitset_t& triggers );

    };


    template <typename method_signature_t>
        requires std::derived_from<typename binding::method_traits<method_signature_t>::class_t, PlayerController>
    void InputMappingContext::bind_to_input_action(
        typename binding::method_traits<method_signature_t>::class_t& controller,
        UID uid, method_signature_t command,
        const binding::trigger_bitset_t& triggers )
    {
        using traits = binding::method_traits<method_signature_t>;
        bind_to_input_action_impl( controller, uid,
                                   std::function<void( typename traits::param_t )>{
                                       std::bind( command, &controller, std::placeholders::_1 )
                                   },
                                   triggers );
    }

}


#endif //!INPUTMAPPINGCONTEXT_H
