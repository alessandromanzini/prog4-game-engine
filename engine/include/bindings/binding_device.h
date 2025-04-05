#ifndef BINDINGDEVICE_H
#define BINDINGDEVICE_H

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <bindings/binding_types.h>
#include <controllers/PlayerController.h>
#include <framework/UID.h>

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <deque>
#include <list>


namespace engine::binding
{
    class CommandSet final
    {
        using command_list_t = std::list<input_command_variant_t>;

    public:
        void set( CommandInfo&& info );
        void execute( input_value_variant_t value, TriggerEvent trigger ) const;

    private:
        command_list_t triggered_commands_{};
        command_list_t pressed_commands_{};
        command_list_t released_commands_{};

        [[nodiscard]] const command_list_t& select_command_list( TriggerEvent trigger ) const;

    };


    struct DeviceInfo
    {
        DeviceType type{};
        uint8_t id{ std::numeric_limits<decltype( id )>::max( ) };
    };


    class DeviceContext final
    {
    public:
        DeviceContext( PlayerController& controller, DeviceInfo deviceInfo );

        [[nodiscard]] PlayerController& get_controller( );
        [[nodiscard]] const PlayerController& get_controller( ) const;

        [[nodiscard]] bool is_device_suitable( DeviceInfo deviceInfo ) const;

        void bind_command( UID uid, CommandInfo&& commandInfo );
        void unbind_command( UID uid );

        // Signals the input action linked to the command, if previously bound.
        void signal_input( const InputSnapshot& input );

        // Dispatches all signaled inputs to the corresponding commands.
        void execute_commands( );

        [[nodiscard]] const DeviceInfo& get_device_info( ) const { return device_info_; }

    private:
        const DeviceInfo device_info_{};

        PlayerController& controller_;

        std::deque<InputSnapshot> signaled_inputs_queue_{};
        std::unordered_map<UID, CommandSet> command_sets_{};

        void execute_commands_on_trigger( const InputSnapshot& input, TriggerEvent trigger ) const;

    };

}


#endif //!BINDINGDEVICE_H
