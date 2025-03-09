#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include "Singleton.h"
#include "type_utility.hpp"
#include "bindings/InputMappingContext.h"

// +--------------------------------+
// | GLM HEADERS					|
// +--------------------------------+
#include <glm.hpp>

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <memory>

namespace engine
{
	typedef unsigned short BTN_MASK;
	enum class BTN_MASK_CODE : BTN_MASK
	{
		BTN_DPAD_UP		= 0x0001,
		BTN_DPAD_DOWN   = 0x0002,
		BTN_DPAD_LEFT   = 0x0004, 
		BTN_DPAD_RIGHT  = 0x0008,
		BTN_A			= 0x1000,
		BTN_B			= 0x2000,
		BTN_X			= 0x4000,
		BTN_Y			= 0x8000
	};

	inline BTN_MASK code_to_mask( BTN_MASK_CODE code )
	{
		return static_cast<BTN_MASK>( code );
	}

	class InputSystem final : public Singleton<InputSystem>
	{
	public:
		InputSystem( );

		bool process_input( );

		[[nodiscard]] bool is_controller_connected( ) const;

		[[nodiscard]] bool is_button_pressed( BTN_MASK_CODE mask, bool strict = false ) const;
		[[nodiscard]] bool is_button_released( BTN_MASK_CODE mask, bool strict = false ) const;
		[[nodiscard]] bool is_button_down( BTN_MASK_CODE mask, bool strict = false ) const;

		glm::vec2 get_LX( ) const;
		glm::vec2 get_RX( ) const;

		InputMappingContext& get_input_mapping_context( );

	private:
		class InputSystemImpl;
		std::unique_ptr<InputSystemImpl> impl_ptr_{ nullptr };

	};

	// +--------------------------------+
	// | OPERATOR OVERLOADING			|
	// +--------------------------------+
	inline BTN_MASK_CODE operator|( BTN_MASK_CODE lhs, BTN_MASK_CODE rhs )
	{
		return static_cast<BTN_MASK_CODE>( static_cast<std::underlying_type<BTN_MASK_CODE>::type>( lhs ) |
										   static_cast<std::underlying_type<BTN_MASK_CODE>::type>( rhs ) );
	}

	extern InputSystem& INPUT_SYSTEM;

}

#endif // INPUTSYSTEM_H
