#include "singletons/InputSystem.h"

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include "bindings/InputMappingContext.h"
#include "singletons/UIController.h"

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
            const auto key       = static_cast<binding::key_t>( e.key.keysym.sym );
            const auto btn       = static_cast<binding::btn_t>( e.cbutton.button );
            const auto gamepadId = static_cast<uint8_t>( e.cdevice.which );

            switch ( e.type )
            {
                case SDL_QUIT:
                    request_quit_ = true;
                    return;

                case SDL_KEYDOWN:
                    trigger( binding::UniformBindingCode( key ), { binding::DeviceType::KEYBOARD } );
                    break;

                case SDL_KEYUP:
                    release( binding::UniformBindingCode( key ), { binding::DeviceType::KEYBOARD } );
                    break;

                case SDL_CONTROLLERBUTTONDOWN:
                    trigger( binding::UniformBindingCode( btn ), { binding::DeviceType::GAMEPAD, gamepadId } );
                    break;

                case SDL_CONTROLLERBUTTONUP:
                    trigger( binding::UniformBindingCode( btn ), { binding::DeviceType::KEYBOARD, gamepadId } );
                    break;

                default:
                    break;
            }

            // Process events for ImGui
            UI_CONTROLLER.process_input( e );
        }

        // Dispatch all keys and buttons pressed every frame
        press( );
    }


    void InputSystem::forward_code_to_contexts( const binding::UniformBindingCode code, const binding::TriggerEvent trigger,
                                                const binding::DeviceInfo deviceInfo )
    {
        input_mapping_context_.signal( code, trigger, deviceInfo );
    }


    void InputSystem::trigger( binding::UniformBindingCode code, const binding::DeviceInfo deviceInfo )
    {
        switch ( deviceInfo.type )
        {
            case binding::DeviceType::KEYBOARD:
                if ( not keys_pressed_this_frame_.contains( static_cast<binding::key_t>( code.code ) ) )
                {
                    forward_code_to_contexts( code, TriggerEvent::TRIGGERED, deviceInfo );
                    keys_pressed_this_frame_.insert( static_cast<binding::key_t>( code.code ) );
                }
                break;
            case binding::DeviceType::GAMEPAD:
                if ( not buttons_pressed_this_frame_.contains( static_cast<binding::btn_t>( code.code ) ) )
                {
                    forward_code_to_contexts( code, TriggerEvent::TRIGGERED, deviceInfo );
                    buttons_pressed_this_frame_.insert( static_cast<binding::btn_t>( code.code ) );
                }
                break;
        }
    }


    void InputSystem::release( binding::UniformBindingCode code, const binding::DeviceInfo deviceInfo )
    {
        switch ( deviceInfo.type )
        {
            case binding::DeviceType::KEYBOARD:
                keys_pressed_this_frame_.erase( static_cast<binding::key_t>( code.code ) );
                break;
            case binding::DeviceType::GAMEPAD:
                buttons_pressed_this_frame_.erase( static_cast<binding::btn_t>( code.code ) );
                break;
        }

        forward_code_to_contexts( code, TriggerEvent::RELEASED, deviceInfo );
    }


    void InputSystem::press( )
    {
        // TODO: Implement this function to handle the press event
        // for ( const auto key : keys_pressed_this_frame_ )
        // {
        //     input_mapping_context_.signal( key, true, TriggerEvent::PRESSED );
        // }
        //
        // for ( const auto button : buttons_pressed_this_frame_ )
        // {
        //     input_mapping_context_.signal( button, true, TriggerEvent::PRESSED );
        // }
    }

}
