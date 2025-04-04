#ifndef BINDINGDEVICE_H
#define BINDINGDEVICE_H

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <bindings/binding_structs.h>
#include <bindings/binding_types.h>
#include <controllers/PlayerController.h>
#include <framework/UID.h>

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <list>


namespace engine::binding
{
    class CommandSet final
    {
    public:
        void set( CommandInfo&& info );
        void execute( input_value_variant_t value, TriggerEvent trigger ) const;

    private:
        std::list<input_command_variant_t> triggered_commands_{};
        std::list<input_command_variant_t> pressed_commands_{};
        std::list<input_command_variant_t> released_commands_{};

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

        void execute_commands( UID uid, bool value, TriggerEvent trigger ) const;

        [[nodiscard]] const DeviceInfo& get_device_info( ) const { return device_info_; }

    private:
        const DeviceInfo device_info_{};

        PlayerController& controller_;

        std::unordered_map<UID, CommandSet> command_sets_{};

    };

}


#endif //!BINDINGDEVICE_H
