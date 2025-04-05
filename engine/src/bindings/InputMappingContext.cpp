#include <bindings/InputMappingContext.h>

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <bindings/binding_controls.h>

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <cassert>

using namespace engine::binding;


namespace engine
{
    // +--------------------------------+
    // | PUBLIC SCOPE					|
    // +--------------------------------+
    void InputMappingContext::register_input_action( const InputAction& action, const UniformBindingCode code )
    {
        assert( not action_binds_.contains( code ) && "Input action already registered on this code!" );
        action_binds_[code].push_back( action );
    }


    void InputMappingContext::register_input_action( const UID uid, const UniformBindingCode code )
    {
        register_input_action( InputAction{ uid }, code );
    }


    void InputMappingContext::register_device( PlayerController& controller, const DeviceInfo deviceInfo )
    {
        device_contexts_.emplace_back( controller, deviceInfo );
    }


    void InputMappingContext::unregister_device( const PlayerController& controller )
    {
        if ( const auto it = find_device_context( controller ); it.has_value( ) )
        {
            device_contexts_.erase( it.value( ) );
        }
    }


    void InputMappingContext::signal( const UniformBindingCode code, const TriggerEvent trigger, const DeviceInfo deviceInfo )
    {
        if ( not action_binds_.contains( code ) )
        {
            return;
        }

        auto& actions = action_binds_.at( code );
        const bool value = trigger_to_value( trigger );

        // For every input action bound to the code, we signal the device context to execute the commands
        for ( auto& device : device_contexts_ )
        {
            if ( not device.is_device_suitable( deviceInfo ) )
            {
                continue;
            }

            for ( const auto& [uid, modifiers] : actions )
            {
                device.signal_input( { uid, value, bitset_cast( trigger ) } );
            }
        }
    }


    void InputMappingContext::dispatch( )
    {
        // Dispatch all accumulated inputs
        for ( auto& device : device_contexts_ )
        {
            device.execute_commands( );
        }
    }


    const std::list<DeviceContext>& InputMappingContext::get_devices( ) const
    {
        return device_contexts_;
    }


    // +--------------------------------+
    // | PRIVATE SCOPE					|
    // +--------------------------------+
    InputMappingContext::optional_device_it InputMappingContext::find_device_context(
        const PlayerController& controller )
    {
        const auto it = std::ranges::find_if( device_contexts_,
                                              [&controller]( const auto& context )
                                                  {
                                                      return context.get_controller( ) == controller;
                                                  } );

        if ( it != device_contexts_.end( ) )
        {
            return it;
        }
        return std::nullopt;
    }


    void InputMappingContext::bind_to_input_action_impl( const PlayerController& controller, const UID uid,
                                                         input_command_variant_t&& command,
                                                         const TriggerEvent trigger )
    {
        const auto deviceIt = find_device_context( controller );
        assert( deviceIt.has_value() && "Device context not found!" );

        deviceIt.value( )->bind_command( uid, { std::move( command ), trigger } );
    }

}
