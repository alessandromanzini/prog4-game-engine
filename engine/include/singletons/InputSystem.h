#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include "Singleton.h"

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
		BTN_A = 0x1000,
		BTN_B = 0x2000
	};

	// Define the bitwise OR operator for the enum class.
	inline BTN_MASK_CODE operator|( BTN_MASK_CODE lhs, BTN_MASK_CODE rhs )
	{
		return static_cast<BTN_MASK_CODE>( static_cast<std::underlying_type<BTN_MASK_CODE>::type>( lhs ) |
								  static_cast<std::underlying_type<BTN_MASK_CODE>::type>( rhs ) );
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

	private:
		class InputSystemImpl;
		std::unique_ptr<InputSystemImpl> impl_ptr_{ nullptr };

	};

	extern InputSystem& INPUT_SYSTEM;

}

#endif // INPUTSYSTEM_H
