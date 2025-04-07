#include <singletons/InputSystem.h>

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <bindings/binding_device.h>
#include <bindings/InputMappingContext.h>
#include <singletons/UIController.h>

// +--------------------------------+
// | SDL HEADERS					|
// +--------------------------------+
#include <SDL.h>

// +--------------------------------+
// | GLM HEADERS					|
// +--------------------------------+
#include <glm.hpp>

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <algorithm>
#include <cassert>
#include <vector>

using namespace engine::binding;


namespace engine
{
    InputSystem& INPUT_SYSTEM = InputSystem::get_instance( );


    bool InputSystem::process_input( )
    {
        // Poll SDL events
        poll( );

        // Dispatch all input actions
        input_mapping_context_.dispatch( );

        return !request_quit_;
    }


    InputSystem::device_id_t InputSystem::fetch_free_gamepad_id( ) const
    {
        auto ids{ get_connected_gamepad_ids( ) };

        // Remove already-used gamepad IDs
        for ( const auto& device : input_mapping_context_.get_devices( ) )
        {
            if ( device.get_device_info( ).type == DeviceType::GAMEPAD )
            {
                std::erase( ids, device.get_device_info( ).id );
            }
            if ( ids.empty( ) )
            {
                break;
            }
        }

        // Throw an error if no gamepad ID is available
        if ( ids.empty( ) )
        {
            throw std::runtime_error( "No gamepad id available." );
        }

        // Return the first one (they're already sorted)
        return ids.front( );
    }


    InputMappingContext& InputSystem::get_input_mapping_context( )
    {
        return input_mapping_context_;
    }


    void InputSystem::poll( )
    {
        SDL_Event e;
        while ( SDL_PollEvent( &e ) )
        {
            const UniformBindingCode key{ static_cast<binding::key_t>( e.key.keysym.sym ) };
            const UniformBindingCode btn{ static_cast<binding::btn_t>( e.cbutton.button ) };

            const auto gamepadId{ static_cast<uint8_t>( e.cdevice.which ) };

            switch ( e.type )
            {
                case SDL_QUIT:
                    request_quit_ = true;
                    return;

                case SDL_KEYDOWN:
                    forward_code_to_contexts( key, TriggerEvent::TRIGGERED, { DeviceType::KEYBOARD } );
                    break;

                case SDL_KEYUP:
                    forward_code_to_contexts( key, TriggerEvent::RELEASED, { DeviceType::KEYBOARD } );
                    break;

                case SDL_CONTROLLERBUTTONDOWN:
                    forward_code_to_contexts( btn, TriggerEvent::TRIGGERED, { DeviceType::GAMEPAD, gamepadId } );
                    break;

                case SDL_CONTROLLERBUTTONUP:
                    forward_code_to_contexts( btn, TriggerEvent::RELEASED, { DeviceType::GAMEPAD, gamepadId } );
                    break;

                default:
                    break;
            }

            // Process events for ImGui
            UI_CONTROLLER.process_input( e );
        }

        // Dispatch all keys and buttons pressed every frame
        forward_state_to_contexts( DeviceType::KEYBOARD );
        forward_state_to_contexts( DeviceType::GAMEPAD );
    }


    void InputSystem::forward_code_to_contexts( const UniformBindingCode code,
                                                const TriggerEvent trigger,
                                                const DeviceInfo deviceInfo )
    {
        InputBuffer& buffer{ select_buffer( deviceInfo.type ) };

        // ReSharper disable once CppIncompleteSwitchStatement
        // ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
        switch ( trigger )
        {
            case TriggerEvent::TRIGGERED:
                // We must trigger only if the key/button is not already pressed (was released)
                if ( not buffer.is_pressed( code, deviceInfo.id ) )
                {
                    buffer.trigger( code, deviceInfo.id );
                    input_mapping_context_.signal( code, TriggerEvent::TRIGGERED, deviceInfo );
                }
                break;
            case TriggerEvent::PRESSED:
                input_mapping_context_.signal( code, TriggerEvent::PRESSED, deviceInfo );
                break;
            case TriggerEvent::RELEASED:
                buffer.release( code, deviceInfo.id );
                input_mapping_context_.signal( code, TriggerEvent::RELEASED, deviceInfo );
                break;
        }
    }


    void InputSystem::forward_state_to_contexts( const DeviceType deviceType )
    {
        const InputBuffer& buffer{ select_buffer( deviceType ) };
        for ( const auto& [code, deviceId] : buffer.get_pressed_this_frame( ) )
        {
            input_mapping_context_.signal( code, TriggerEvent::PRESSED, { deviceType, deviceId } );
        }
    }


    InputBuffer& InputSystem::select_buffer( const DeviceType deviceType )
    {
        switch ( deviceType )
        {
            case DeviceType::KEYBOARD:
                return keyboard_buffer_;
            case DeviceType::GAMEPAD:
                return gamepad_buffer_;
        }
		throw std::invalid_argument( "Invalid device type." );
    }


    std::vector<InputSystem::device_id_t> InputSystem::get_connected_gamepad_ids( )
    {
        std::vector<device_id_t> gamepadIds{};

        for ( int i{}; i < SDL_NumJoysticks( ); ++i )
        {
            if ( SDL_IsGameController( i ) )
            {
                gamepadIds.push_back( static_cast<device_id_t>( i ) );
            }
        }

        return gamepadIds;
    }

}
