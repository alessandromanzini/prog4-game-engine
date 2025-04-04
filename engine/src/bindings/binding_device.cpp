// +---------------------------+
// | PROJECT HEADERS           |
// +---------------------------+
#include <bindings/binding_controls.h>
#include <bindings/binding_device.h>

// +---------------------------+
// | STANDARD HEADERS          |
// +---------------------------+
#include <algorithm>


namespace engine::binding
{
    // +---------------------------+
    // | COMMAND SET               |
    // +---------------------------+
    void CommandSet::set( CommandInfo&& info )
    {
        switch ( info.trigger )
        {
            case TriggerEvent::TRIGGERED:
                triggered_commands_.push_back( std::move( info.command ) );
                break;
            case TriggerEvent::PRESSED:
                pressed_commands_.push_back( std::move( info.command ) );
                break;
            case TriggerEvent::RELEASED:
                released_commands_.push_back( std::move( info.command ) );
                break;
            default:
                assert( false && "Invalid trigger event!" );
        }
    }


    void CommandSet::execute( const input_value_variant_t value, const TriggerEvent trigger ) const
    {
        // TODO: Implement correct function call
        switch ( trigger )
        {
            case TriggerEvent::TRIGGERED:
                for ( const auto& command : triggered_commands_ )
                {
                    std::get<std::function<void( bool )>>( command )( std::get<bool>( value ) );
                }
                break;
            case TriggerEvent::PRESSED:
                // for ( const auto& command : triggered_commands_ )
                // {
                //     command( value );
                // }
                break;
            case TriggerEvent::RELEASED:
                // for ( const auto& command : released_commands_ )
                // {
                //     command( value );
                // }
                break;
            default:
                assert( false && "Invalid trigger event!" );
        }
    }


    // +---------------------------+
    // | DEVICE CONTEXT            |
    // +---------------------------+
    DeviceContext::DeviceContext( PlayerController& controller, DeviceInfo deviceInfo )
        : device_info_{ std::move( deviceInfo ) }
        , controller_{ controller } { }


    PlayerController& DeviceContext::get_controller( )
    {
        return controller_;
    }


    const PlayerController& DeviceContext::get_controller( ) const
    {
        return controller_;
    }


    bool DeviceContext::is_device_suitable( const DeviceInfo deviceInfo ) const
    {
        switch ( deviceInfo.type )
        {
            case DeviceType::KEYBOARD:
                return device_info_.type == DeviceType::KEYBOARD;
            case DeviceType::GAMEPAD:
                return device_info_.type == DeviceType::GAMEPAD && device_info_.id == deviceInfo.id;
            default:
                return false;
        }
    }


    void DeviceContext::bind_command( const UID uid, CommandInfo&& commandInfo )
    {
        auto& commands = command_sets_[uid];
        commands.set( std::move( commandInfo ) );
    }


    void DeviceContext::execute_commands( const UID uid, const bool value, const TriggerEvent trigger ) const
    {
        if ( not command_sets_.contains( uid ) )
        {
            return;
        }
        command_sets_.at( uid ).execute( value, trigger );
    }

}
