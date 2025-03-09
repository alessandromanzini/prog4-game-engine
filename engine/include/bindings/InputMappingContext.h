#ifndef INPUTMAPPINGCONTEXT_H
#define INPUTMAPPINGCONTEXT_H

#include "InputActionBuilder.h"

// +--------------------------------+
// | SDL HEADERS					|
// +--------------------------------+
#include <SDL.h>

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <functional>
#include <variant>
#include <unordered_map>
#include <vector>

namespace engine
{
	class InputMappingContext final
	{
	public:
		InputMappingContext( ) = default;
		virtual ~InputMappingContext( ) noexcept = default;

		template <DerivedInputActionBuilder builder_t>
		void bind_input_action( unsigned short mask, const std::function<void( typename builder_t::value_type )>& command, InputAction<typename builder_t::value_type> overrides = {} );

		template <DerivedInputActionBuilder builder_t>
		void bind_input_action( SDL_Keycode code, const std::function<void( typename builder_t::value_type )>& command, InputAction<typename builder_t::value_type> overrides = {} );

		void dispatch( unsigned short input, action_value_variant_t value, TriggerEvent trigger ) const;
		void dispatch( SDL_Keycode input, action_value_variant_t value, TriggerEvent trigger ) const;

	private:
		std::unordered_map<unsigned short, std::vector<command_variant_t>> button_pressed_commands_{};
		std::unordered_map<unsigned short, std::vector<command_variant_t>> button_released_commands_{};

		std::unordered_map<SDL_Keycode, std::vector<command_variant_t>> key_pressed_commands_{};
		std::unordered_map<SDL_Keycode, std::vector<command_variant_t>> key_released_commands_{};

	};

	template <DerivedInputActionBuilder builder_t>
	void InputMappingContext::bind_input_action( unsigned short mask, const std::function<void( typename builder_t::value_type )>& command, InputAction<typename builder_t::value_type> /* overrides = {} */ )
	{
		// TODO: implement modifiers, overrides

		// Build the input action and the binding
		InputAction<typename builder_t::value_type> ia;
		builder_t{}.build( ia );

		if ( ia.triggers.test( static_cast<trigger_mask_t>( TriggerEvent::Pressed ) ) )
		{
			button_pressed_commands_[mask].push_back( command );
		}
		if ( ia.triggers.test( static_cast<trigger_mask_t>( TriggerEvent::Released ) ) )
		{
			button_released_commands_[mask].push_back( command );
		}
	}

	template <DerivedInputActionBuilder builder_t>
	void InputMappingContext::bind_input_action( SDL_Keycode code, const std::function<void( typename builder_t::value_type )>& command, InputAction<typename builder_t::value_type> /* overrides = {} */ )
	{
		// TODO: implement modifiers, overrides

		// Build the input action and the binding
		InputAction<typename builder_t::value_type> ia;
		builder_t{}.build( ia );

		if ( ia.triggers.test( static_cast<trigger_mask_t>( TriggerEvent::Pressed ) ) )
		{
			key_pressed_commands_[code].push_back( command );
		}
		if ( ia.triggers.test( static_cast<trigger_mask_t>( TriggerEvent::Released ) ) )
		{
			key_released_commands_[code].push_back( command );
		}
	}

	// +--------------------------------+
	// | CONCEPTS						|
	// +--------------------------------+
	template <typename context_t>
	concept DerivedInputMappingContext = std::derived_from<context_t, InputMappingContext>;

}

#endif // !INPUTMAPPINGCONTEXT_H
