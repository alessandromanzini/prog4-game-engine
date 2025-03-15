#include "bindings/InputMappingContext.h"

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include "bindings/Command.h"

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <cassert>
#include <algorithm>

using namespace engine::binding;

namespace engine
{
	// +--------------------------------+
	// | PUBLIC SCOPE					|
	// +--------------------------------+
	void InputMappingContext::register_input_action( UID uid, binding::key_t key, input_action_variant_t inputAction, const binding::trigger_bitset_t& triggers )
	{
		assert( not is_input_action_registered_for_key( uid, key ) && "Input action already registered on this key!" );
		register_input_action( uid, inputAction, triggers, keyboard_actions_[key] );
	}


	void InputMappingContext::register_input_action( UID uid, binding::btn_t button, input_action_variant_t inputAction, const binding::trigger_bitset_t& triggers )
	{
		assert( not is_input_action_registered_for_button( uid, button ) && "Input action already registered on this button!" );
		register_input_action( uid, inputAction, triggers, gamepad_actions_[button] );
	}


	void InputMappingContext::bind_input_action( UID uid, Command* pCommand )
	{
		assert( is_input_action_registered_for_any( uid ) && "Input action has not been registered!" );
		
		// Iterate over all triggers and add the action to the corresponding list
		const auto& triggers = action_triggers_[uid];
		if ( triggers.test( seq_mask_cast( TriggerEvent::TRIGGERED ) ) )
		{
			triggered_commands_[uid].push_back( pCommand );
		}
		if ( triggers.test( seq_mask_cast( TriggerEvent::PRESSED ) ) )
		{
			pressed_commands_[uid].push_back( pCommand );
		}
		if ( triggers.test( seq_mask_cast( TriggerEvent::RELEASED ) ) )
		{
			released_commands_[uid].push_back( pCommand );
		}
	}


	void InputMappingContext::unbind_input_action( UID uid, Command* pCommand )
	{
		assert( is_input_action_registered_for_any( uid ) && "Input action was not found!" );

		std::erase( triggered_commands_[uid], pCommand );
		std::erase( pressed_commands_[uid], pCommand );
		std::erase( released_commands_[uid], pCommand );
	}


	void InputMappingContext::signal( binding::key_t key, bool value, binding::TriggerEvent trigger )
	{
		for ( auto& action : keyboard_actions_[key] )
		{
			signal( action.uid, value, trigger );
		}
	}


	void InputMappingContext::signal( binding::btn_t button, bool value, binding::TriggerEvent trigger )
	{
		for ( auto& action : gamepad_actions_[button] )
		{
			signal( action.uid, value, trigger );
		}
	}


	void InputMappingContext::dispatch( ) const
	{
		for ( auto& input : signaled_inputs_ )
		{
			if ( not pressed_commands_.contains( input.uid ) )
			{
				continue;
			}

			// TODO: manage trigger events
			for ( auto* pCommand : pressed_commands_.at( input.uid ) )
			{
				pCommand->execute( );
			}
		}
	}


	// +--------------------------------+
	// | PRIVATE SCOPE					|
	// +--------------------------------+
	void InputMappingContext::register_input_action( UID uid, input_action_variant_t inputAction, const binding::trigger_bitset_t& triggers, std::vector<InputActionBinding>& actions )
	{
		assert( not triggers.none( ) && "No trigger has been set for action!" );

		// Register binding and triggers
		actions.push_back( InputActionBinding{ uid, inputAction } );
		action_triggers_[uid] |= triggers;
	}


	void InputMappingContext::signal( UID uid, bool value, binding::TriggerEvent trigger )
	{
		auto it = std::find_if( signaled_inputs_.begin( ), signaled_inputs_.end( ), [uid]( const auto& context ) {
			return context.uid == uid;
		} );

		trigger_bitset_t trigger_mask = bitset_cast( trigger );

		// If the input was not signaled yet, add it to the list
		// Otherwise, combine the value
		if ( it == signaled_inputs_.end( ) )
		{
			signaled_inputs_.push_back( { uid, value, trigger_mask } );
		}
		else
		{
			// TODO: actually combine the values
			it->value = value;
			it->triggers |= trigger_mask;
		}
	}


	bool InputMappingContext::is_input_action_registered( UID uid, const std::vector<InputActionBinding>& bindings ) const
	{
		return std::any_of( bindings.begin( ), bindings.end( ), [uid]( const auto& binding )
			{
				return binding.uid == uid;
			} );
	}


	bool InputMappingContext::is_input_action_registered_for_any( UID uid ) const
	{
		return std::any_of( keyboard_actions_.begin( ), keyboard_actions_.end( ), [this, uid]( const auto& pair )
			{
				return is_input_action_registered( uid, pair.second );
			} )
			|| std::any_of( gamepad_actions_.begin( ), gamepad_actions_.end( ), [this, uid]( const auto& pair )
			{
				return is_input_action_registered( uid, pair.second );
			} );
	}


	bool InputMappingContext::is_input_action_registered_for_key( UID uid, binding::key_t key ) const
	{
		auto it = keyboard_actions_.find( key );
		if ( it == keyboard_actions_.end( ) )
		{
			return false;
		}
		return is_input_action_registered( uid, it->second );
	}


	bool InputMappingContext::is_input_action_registered_for_button( UID uid, binding::btn_t button ) const
	{
		auto it = gamepad_actions_.find( button );
		if ( it == gamepad_actions_.end( ) )
		{
			return false;
		}
		return is_input_action_registered( uid, it->second );
	}

}
