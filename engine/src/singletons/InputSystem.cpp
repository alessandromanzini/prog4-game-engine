#include <singletons/InputSystem.h>

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
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
#include <binding_device.h>
#include <cassert>
#include <vector>


using TriggerEvent = engine::binding::TriggerEvent;

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


    InputMappingContext& InputSystem::get_input_mapping_context( )
    {
        return input_mapping_context_;
    }


    void InputSystem::poll( )
    {
        SDL_Event e;
        while ( SDL_PollEvent( &e ) )
        {
            const binding::UniformBindingCode key{ static_cast<binding::key_t>( e.key.keysym.sym ) };
            const binding::UniformBindingCode btn{ static_cast<binding::btn_t>( e.cbutton.button ) };

            const auto gamepadId{ static_cast<uint8_t>( e.cdevice.which ) };

            switch ( e.type )
            {
                case SDL_QUIT:
                    request_quit_ = true;
                    return;

                case SDL_KEYDOWN:
                    forward_code_to_contexts( key, TriggerEvent::TRIGGERED, { binding::DeviceType::KEYBOARD } );
                    break;

                case SDL_KEYUP:
                    forward_code_to_contexts( key, TriggerEvent::RELEASED, { binding::DeviceType::KEYBOARD } );
                    break;

                case SDL_CONTROLLERBUTTONDOWN:
                    forward_code_to_contexts( btn, TriggerEvent::TRIGGERED,
                                              { binding::DeviceType::GAMEPAD, gamepadId } );
                    break;

                case SDL_CONTROLLERBUTTONUP:
                    forward_code_to_contexts( btn, TriggerEvent::RELEASED,
                                              { binding::DeviceType::GAMEPAD, gamepadId } );
                    break;

                default:
                    break;
            }

            // Process events for ImGui
            UI_CONTROLLER.process_input( e );
        }

        // Dispatch all keys and buttons pressed every frame
        forward_state_to_contexts( binding::DeviceType::KEYBOARD );
        forward_state_to_contexts( binding::DeviceType::GAMEPAD );
    }


    void InputSystem::forward_code_to_contexts( const binding::UniformBindingCode code,
                                                const binding::TriggerEvent trigger,
                                                const binding::DeviceInfo deviceInfo )
    {
        binding::InputBuffer& buffer{ select_buffer( deviceInfo.type ) };

        // ReSharper disable once CppIncompleteSwitchStatement
        // ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
        switch ( trigger )
        {
            case TriggerEvent::TRIGGERED:
                // We must trigger only if the key/button is not already pressed (was released)
                if ( not buffer.is_pressed( code, deviceInfo.id ) )
                {
                    buffer.trigger( code, deviceInfo.id );
                    input_mapping_context_.signal( code, trigger, deviceInfo );
                }
                break;
            case TriggerEvent::PRESSED:
                input_mapping_context_.signal( code, trigger, deviceInfo );
                break;
            case TriggerEvent::RELEASED:
                buffer.release( code, deviceInfo.id );
                input_mapping_context_.signal( code, trigger, deviceInfo );
                break;
        }
    }


    void InputSystem::forward_state_to_contexts( const binding::DeviceType deviceType )
    {
        const binding::InputBuffer& buffer{ select_buffer( deviceType ) };
        for ( const auto& [code, deviceId] : buffer.get_pressed_this_frame( ) )
        {
            input_mapping_context_.signal( code, TriggerEvent::PRESSED, { deviceType, deviceId } );
        }
    }


    binding::InputBuffer& InputSystem::select_buffer( const binding::DeviceType deviceType )
    {
        switch ( deviceType )
        {
            case binding::DeviceType::KEYBOARD:
                return keyboard_buffer_;
            case binding::DeviceType::GAMEPAD:
                return gamepad_buffer_;
        }
        assert( false && "Invalid device type!" );
    }

}
