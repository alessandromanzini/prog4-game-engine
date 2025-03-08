#include "singletons/InputSystem.h"

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
#include <windows.h>
#include <Xinput.h>
#include <cassert>

namespace engine
{
	InputSystem& INPUT_SYSTEM = InputSystem::get_instance( );

	class InputSystem::InputSystemImpl final
	{
	public:
		InputSystemImpl( )
		{
			capture_controller( );
		}

		[[nodiscard]] bool is_controller_connected( ) const
		{
			return controller_index_ != XUSER_MAX_COUNT;
		}

		bool process_input( )
		{
			SDL_Event e;
			while ( SDL_PollEvent( &e ) )
			{
				if ( e.type == SDL_QUIT )
				{
					return false;
				}

				// Process events for ImGui
				UI_CONTROLLER.process_input( e );
			}

			// Keep capturing controller until one is found
			if ( !validate_controller( controller_index_ ) )
			{
				capture_controller( );
			}
			else
			{
				poll_xinput( );
			}

			return true;
		}

		[[nodiscard]] bool is_button_pressed( WORD mask, bool strict ) const
		{
			WORD evaluation = ( current_state_.Gamepad.wButtons & mask );
			return strict && ( evaluation == mask ) || !strict && evaluation;
		}

		[[nodiscard]] bool is_button_up( WORD mask, bool strict ) const
		{
			WORD evaluation = ( buttons_released_this_frame_ & mask );
			return strict && ( evaluation == mask ) || !strict && evaluation;
		}

		[[nodiscard]] bool is_button_down( WORD mask, bool strict ) const
		{
			WORD evaluation = ( buttons_pressed_this_frame_ & mask );
			return strict && ( evaluation == mask ) || !strict && evaluation;
		}

		const XINPUT_STATE& get_current_state( ) const
		{
			return current_state_;
		}

	private:
		static constexpr SHORT MAX_THUMB_VALUE = 32767;
		static constexpr SHORT DEADZONE = static_cast<SHORT>( 0.24f * MAX_THUMB_VALUE );

		XINPUT_STATE previous_state_{};
		XINPUT_STATE current_state_{};

		DWORD controller_index_{ XUSER_MAX_COUNT };

		WORD buttons_pressed_this_frame_{};
		WORD buttons_released_this_frame_{};

		bool validate_controller( DWORD index ) const
		{
			XINPUT_STATE state;
			ZeroMemory( &state, sizeof( XINPUT_STATE ) );
			return XInputGetState( index, &state ) == ERROR_SUCCESS;
		}

		void capture_controller( )
		{
			controller_index_ = XUSER_MAX_COUNT;
			for ( uint8_t i = 0; i < XUSER_MAX_COUNT; ++i )
			{
				if ( validate_controller( i ) )
				{
					controller_index_ = i;
					break;
				}
			}
		}

		void clear_deadzone( XINPUT_STATE& state ) const
		{
			glm::vec2 lthumb{ state.Gamepad.sThumbLX, state.Gamepad.sThumbLY };
			glm::vec2 rthumb{ state.Gamepad.sThumbRX, state.Gamepad.sThumbRY };

			const float lthumbLength = glm::length( lthumb );
			const float rthumbLength = glm::length( rthumb );

			if ( lthumbLength >= DEADZONE )
			{
				lthumb /= lthumbLength;
				lthumb *= ( lthumbLength - DEADZONE ) / ( MAX_THUMB_VALUE - DEADZONE );
				state.Gamepad.sThumbLX = static_cast<SHORT>( lthumb.x * MAX_THUMB_VALUE );
				state.Gamepad.sThumbLY = static_cast<SHORT>( lthumb.y * MAX_THUMB_VALUE );
			}
			else
			{
				state.Gamepad.sThumbLX = static_cast<SHORT>( 0 );
				state.Gamepad.sThumbLY = static_cast<SHORT>( 0 );
			}

			if ( rthumbLength >= DEADZONE )
			{
				rthumb /= rthumbLength;
				rthumb *= ( rthumbLength - DEADZONE ) / ( MAX_THUMB_VALUE - DEADZONE );
				state.Gamepad.sThumbRX = static_cast<SHORT>( rthumb.x * MAX_THUMB_VALUE );
				state.Gamepad.sThumbRY = static_cast<SHORT>( rthumb.y * MAX_THUMB_VALUE );
			}
			else
			{
				state.Gamepad.sThumbRX = static_cast<SHORT>( 0 );
				state.Gamepad.sThumbRY = static_cast<SHORT>( 0 );
			}
		}

		void poll_xinput( )
		{
			// Save to the previous state and reset the current state
			CopyMemory( &previous_state_, &current_state_, sizeof( XINPUT_STATE ) );
			ZeroMemory( &current_state_, sizeof( XINPUT_STATE ) );

			XInputGetState( controller_index_, &current_state_ );
			clear_deadzone( current_state_ );

			// XOR between previous and current to get differences
			WORD buttonChanges = current_state_.Gamepad.wButtons ^ previous_state_.Gamepad.wButtons;

			// AND between changes and current to get the buttons pressed this frame
			buttons_pressed_this_frame_ = buttonChanges & current_state_.Gamepad.wButtons;
			// AND between changes and NOT current to get the buttons release this frame
			buttons_released_this_frame_ = buttonChanges & ( ~current_state_.Gamepad.wButtons );
		}

	};

	InputSystem::InputSystem( )
		: impl_ptr_{ std::make_unique<InputSystemImpl>( ) }
	{
	}

	bool InputSystem::process_input( )
	{
		return impl_ptr_->process_input( );
	}

	bool InputSystem::is_controller_connected( ) const
	{
		return impl_ptr_->is_controller_connected( );
	}

	bool InputSystem::is_button_pressed( BTN_MASK_CODE mask, bool strict /*= false */ ) const
	{
		return impl_ptr_->is_button_pressed( static_cast<BTN_MASK>( mask ), strict );
	}

	bool InputSystem::is_button_released( BTN_MASK_CODE mask, bool strict /*= false */ ) const
	{
		return impl_ptr_->is_button_up( static_cast<BTN_MASK>( mask ), strict );
	}

	bool InputSystem::is_button_down( BTN_MASK_CODE mask, bool strict /*= false */ ) const
	{
		return impl_ptr_->is_button_down( static_cast<BTN_MASK>( mask ), strict );
	}

	glm::vec2 InputSystem::get_LX( ) const
	{
		const XINPUT_STATE& state = impl_ptr_->get_current_state( );
		return { state.Gamepad.sThumbLX, state.Gamepad.sThumbLY };
	}

	glm::vec2 InputSystem::get_RX( ) const
	{
		const XINPUT_STATE& state = impl_ptr_->get_current_state( );
		return { state.Gamepad.sThumbRX, state.Gamepad.sThumbRY };
	}

}
