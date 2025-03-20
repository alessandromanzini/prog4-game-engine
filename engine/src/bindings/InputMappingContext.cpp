#include "bindings/InputMappingContext.h"

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include "bindings/Command.h"

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <algorithm>
#include <cassert>


using namespace engine::binding;

namespace engine
{
    // +--------------------------------+
    // | PUBLIC SCOPE					|
    // +--------------------------------+
    void InputMappingContext::register_input_action( const UID uid, const binding::key_t key,
                                                     const input_action_variant_t inputAction,
                                                     const binding::trigger_bitset_t& triggers )
    {
        assert( not is_input_action_registered_for_key( uid, key ) && "Input action already registered on this key!" );
        register_input_action( uid, inputAction, triggers, keyboard_actions_[key] );
    }


    void InputMappingContext::register_input_action( const UID uid, const binding::btn_t button,
                                                     const input_action_variant_t inputAction,
                                                     const binding::trigger_bitset_t& triggers )
    {
        assert(
            not is_input_action_registered_for_button( uid, button ) &&
            "Input action already registered on this button!" );
        register_input_action( uid, inputAction, triggers, gamepad_actions_[button] );
    }


    void InputMappingContext::bind_input_action( const UID uid, Command* command )
    {
        assert( is_input_action_registered_for_any( uid ) && "Input action has not been registered!" );

        // Iterate over all triggers and add the action to the corresponding list
        const auto& triggers = action_triggers_[uid];
        if ( triggers.test( seq_mask_cast( TriggerEvent::TRIGGERED ) ) )
        {
            triggered_commands_[uid].push_back( command );
        }
        if ( triggers.test( seq_mask_cast( TriggerEvent::PRESSED ) ) )
        {
            pressed_commands_[uid].push_back( command );
        }
        if ( triggers.test( seq_mask_cast( TriggerEvent::RELEASED ) ) )
        {
            released_commands_[uid].push_back( command );
        }
    }


    void InputMappingContext::unbind_input_action( const UID uid, Command* command )
    {
        assert( is_input_action_registered_for_any( uid ) && "Input action was not found!" );

        std::erase( triggered_commands_[uid], command );
        std::erase( pressed_commands_[uid], command );
        std::erase( released_commands_[uid], command );
    }


    void InputMappingContext::signal( const binding::key_t key, const bool value, const binding::TriggerEvent trigger )
    {
        for ( auto& action : keyboard_actions_[key] )
        {
            signal( action.uid, value, trigger );
        }
    }


    void InputMappingContext::signal( const binding::btn_t button, const bool value,
                                      const binding::TriggerEvent trigger )
    {
        for ( auto& [uid, input_action] : gamepad_actions_[button] )
        {
            signal( uid, value, trigger );
        }
    }


    void InputMappingContext::dispatch( )
    {
        // TODO: manage all events
        // TRIGGERED
        for ( auto& input : signaled_triggered_inputs_ )
        {
            if ( not triggered_commands_.contains( input.uid ) )
            {
                continue;
            }

            if ( triggered_commands_.contains( input.uid ) )
            {
                for ( auto* command : triggered_commands_.at( input.uid ) )
                {
                    command->execute( );
                }
            }
        }
        signaled_triggered_inputs_.clear( );

        // PRESSED
        for ( auto& input : signaled_pressed_inputs_ )
        {
            if ( not pressed_commands_.contains( input.uid ) )
            {
                continue;
            }

            for ( auto* command : pressed_commands_.at( input.uid ) )
            {
                command->execute( );
            }
        }
        signaled_pressed_inputs_.clear( );
    }


    // +--------------------------------+
    // | PRIVATE SCOPE					|
    // +--------------------------------+
    void InputMappingContext::register_input_action( const UID uid, const input_action_variant_t inputAction,
                                                     const binding::trigger_bitset_t& triggers,
                                                     std::vector<InputActionBinding>& actions )
    {
        assert( not triggers.none( ) && "No trigger has been set for action!" );

        // Register binding and triggers
        actions.push_back( InputActionBinding{ uid, inputAction } );
        action_triggers_[uid] |= triggers;
    }


    void InputMappingContext::signal( const UID uid, const bool value, const TriggerEvent trigger )
    {
        std::vector<InputActionContext>* signaled_inputs{};
        // TODO: manage bitmask
        switch ( trigger )
        {
            case TriggerEvent::TRIGGERED:
                signaled_inputs = &signaled_triggered_inputs_;
                break;
            case TriggerEvent::PRESSED:
                signaled_inputs = &signaled_pressed_inputs_;
                break;
            case TriggerEvent::RELEASED:
                return;
            case TriggerEvent::ALL:
                return;
        }

        const auto it = std::ranges::find_if(
            *signaled_inputs, [uid]( const auto& context )
                {
                    return context.uid == uid;
                } );

        const trigger_bitset_t trigger_mask = bitset_cast( trigger );

        // If the input was not signaled yet, add it to the list
        // Otherwise, combine the value
        if ( it == signaled_inputs->end( ) )
        {
            signaled_inputs->push_back( { uid, value, trigger_mask } );
        }
        else
        {
            // TODO: actually combine the values
            it->value = value;
            it->triggers |= trigger_mask;
        }
    }


    bool InputMappingContext::is_input_action_registered(
        const UID uid, const std::vector<InputActionBinding>& bindings ) const
    {
        return std::any_of( bindings.begin( ), bindings.end( ), [uid]( const auto& binding )
                                {
                                    return binding.uid == uid;
                                } );
    }


    bool InputMappingContext::is_input_action_registered_for_any( const UID uid ) const
    {
        return std::ranges::any_of( keyboard_actions_, [this, uid]( const auto& pair )
                                        {
                                            return is_input_action_registered( uid, pair.second );
                                        } )
               || std::ranges::any_of( gamepad_actions_, [this, uid]( const auto& pair )
                                           {
                                               return is_input_action_registered( uid, pair.second );
                                           } );
    }


    bool InputMappingContext::is_input_action_registered_for_key( UID uid, binding::key_t key ) const
    {
        const auto it = keyboard_actions_.find( key );
        if ( it == keyboard_actions_.end( ) )
        {
            return false;
        }
        return is_input_action_registered( uid, it->second );
    }


    bool InputMappingContext::is_input_action_registered_for_button( UID uid, binding::btn_t button ) const
    {
        const auto it = gamepad_actions_.find( button );
        if ( it == gamepad_actions_.end( ) )
        {
            return false;
        }
        return is_input_action_registered( uid, it->second );
    }

}
