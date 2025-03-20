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
#include <cassert>
#include <vector>


using TriggerEvent = engine::binding::TriggerEvent;

namespace engine
{
    InputSystem& INPUT_SYSTEM = InputSystem::get_instance( );


    bool InputSystem::process_input( )
    {
        // Poll SDL events
        poll_sdl_events( );

        // Dispatch all input actions
        input_mapping_context_.dispatch( );

        return !request_quit_;
    }


    InputMappingContext& InputSystem::get_input_mapping_context( )
    {
        return input_mapping_context_;
    }


    void InputSystem::poll_sdl_events( )
    {
        SDL_Event e;
        while ( SDL_PollEvent( &e ) )
        {
            const auto key = static_cast<binding::key_t>( e.key.keysym.sym );
            const auto btn = static_cast<binding::btn_t>( e.cbutton.button );

            switch ( e.type )
            {
                case SDL_QUIT:
                    request_quit_ = true;
                    return;

                case SDL_KEYDOWN:
                    press_key( key );
                    break;

                case SDL_KEYUP:
                    release_key( key );
                    break;

                case SDL_CONTROLLERBUTTONDOWN:
                    press_button( btn );
                    break;

                case SDL_CONTROLLERBUTTONUP:
                    release_button( btn );
                    break;

                default:
                    break;
            }

            // Process events for ImGui
            UI_CONTROLLER.process_input( e );
        }

        // Dispatch all keys and buttons pressed every frame
        dispatch_pressed( );
    }


    void InputSystem::press_key( const binding::key_t key )
    {
        if ( keys_pressed_this_frame_.contains( key ) )
        {
            keys_pressed_this_frame_.insert( key );
        }
        else
        {
            input_mapping_context_.signal( key, true, TriggerEvent::TRIGGERED );
        }
    }


    void InputSystem::release_key( const binding::key_t key )
    {
        keys_pressed_this_frame_.erase( key );
        input_mapping_context_.signal( key, false, TriggerEvent::RELEASED );
    }


    void InputSystem::press_button( const binding::btn_t button )
    {
        if ( buttons_pressed_this_frame_.contains( button ) )
        {
            buttons_pressed_this_frame_.insert( button );
        }
        else
        {
            input_mapping_context_.signal( button, true, TriggerEvent::TRIGGERED );
        }
    }


    void InputSystem::release_button( const binding::btn_t button )
    {
        buttons_pressed_this_frame_.erase( button );
        input_mapping_context_.signal( button, false, TriggerEvent::RELEASED );
    }


    void InputSystem::dispatch_pressed( )
    {
        for ( const auto key : keys_pressed_this_frame_ )
        {
            input_mapping_context_.signal( key, true, TriggerEvent::PRESSED );
        }

        for ( const auto button : buttons_pressed_this_frame_ )
        {
            input_mapping_context_.signal( button, true, TriggerEvent::PRESSED );
        }
    }

}
