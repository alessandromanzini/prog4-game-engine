#include "bindings/InputMappingContext.h"

namespace engine
{
	// At the moment, it's only possible to get boolean values from the input system.
	// TODO: support floats and vec2 as well
	void InputMappingContext::dispatch( unsigned short input, action_value_variant_t value, TriggerEvent trigger ) const
	{
		const std::unordered_map<unsigned short, std::vector<command_variant_t>>* pCommands{};

		switch ( trigger )
		{
		case TriggerEvent::Pressed:
			pCommands = &button_pressed_commands_;
			break;
		case TriggerEvent::Released:
			pCommands = &button_released_commands_;
			break;
		}

		// Command resolution should be an addition of multiple input actions
		auto it = pCommands->find( input );
		if ( it != pCommands->end( ) )
		{
			for ( const auto& command : it->second )
			{
				std::visit( [&]( auto&& func ) 
				{
					func( std::get<bool>( value ) );
				}, command );
			}
		}
	}

	void InputMappingContext::dispatch( SDL_Keycode input, action_value_variant_t value, TriggerEvent trigger ) const
	{
		const std::unordered_map<SDL_Keycode, std::vector<command_variant_t>>* pCommands{};

		switch ( trigger )
		{
		case TriggerEvent::Pressed:
			pCommands = &key_pressed_commands_;
			break;
		case TriggerEvent::Released:
			pCommands = &key_released_commands_;
			break;
		}

		// Command resolution should be an addition of multiple input actions
		auto it = pCommands->find( input );
		if ( it != pCommands->end( ) )
		{
			for ( const auto& command : it->second )
			{
				std::visit( [&]( auto&& func )
				{
					func( std::get<bool>( value ) );
				}, command );
			}
		}
	}

}
