// +---------------------------+
// | PROJECT HEADERS           |
// +---------------------------+
#include <bindings/binding_controls.h>
#include <bindings/binding_device.h>

// +---------------------------+
// | STANDARD HEADERS          |
// +---------------------------+


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
                for ( const auto& command : pressed_commands_ )
                {
                    std::get<std::function<void( bool )>>( command )( std::get<bool>( value ) );
                }
                break;
            case TriggerEvent::RELEASED:
                for ( const auto& command : released_commands_ )
                {
                    std::get<std::function<void( bool )>>( command )( std::get<bool>( value ) );
                }
                break;
            default:
                assert( false && "Invalid trigger event!" );
        }
    }


    // +---------------------------+
    // | DEVICE CONTEXT            |
    // +---------------------------+
    DeviceContext::DeviceContext( PlayerController& controller, const DeviceInfo deviceInfo )
        : device_info_{ deviceInfo }
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


    void DeviceContext::signal_input( const InputSnapshot& input )
    {
        if ( not command_sets_.contains( input.uid ) )
        {
            return;
        }

        const auto inputIt = std::ranges::find_if( signaled_inputs_queue_,
                                                   [&]( const auto& current )
                                                       {
                                                           return current.uid == input.uid;
                                                       } );

        // If the input is not already in the queue, we add it
        if ( inputIt == signaled_inputs_queue_.end( ) )
        {
            signaled_inputs_queue_.push_back( input );
        }
        else
        {
            // If the input is already in the queue, we update its value
            merge_value_to_snapshot( *inputIt, input.value );
            inputIt->triggers |= input.triggers;
        }
    }


    void DeviceContext::execute_commands( )
    {
        while ( not signaled_inputs_queue_.empty( ) )
        {
            auto& cumulativeInput = signaled_inputs_queue_.front( );
            signaled_inputs_queue_.pop_front( );

            execute_commands_on_trigger( cumulativeInput, TriggerEvent::TRIGGERED );
            execute_commands_on_trigger( cumulativeInput, TriggerEvent::PRESSED );
            execute_commands_on_trigger( cumulativeInput, TriggerEvent::RELEASED );
        }
    }


    void DeviceContext::execute_commands_on_trigger( const InputSnapshot& input, const TriggerEvent trigger ) const
    {
        if ( input.triggers.test( bit_cast( trigger ) ) )
        {
            command_sets_.at( input.uid ).execute( input.value, trigger );
        }
    }

}
