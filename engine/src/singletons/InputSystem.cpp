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
#include <vector>

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
			bool result = poll_sdl( );

			// Keep capturing controller until one is found
			if ( !validate_controller( controller_index_ ) )
			{
				capture_controller( );
			}
			else
			{
				update_xinput( );
				poll_xinput( );
			}

			return result;
		}

		[[nodiscard]] bool is_button_pressed( WORD mask, bool strict ) const
		{
			WORD evaluation = ( xinput_current_state_.Gamepad.wButtons & mask );
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
			return xinput_current_state_;
		}

		InputMappingContext& get_input_mapping_context( )
		{
			return input_mapping_context_;
		}

	private:
		static constexpr SHORT MAX_THUMB_VALUE = 32767;
		static constexpr SHORT DEADZONE = static_cast<SHORT>( 0.24f * MAX_THUMB_VALUE );

		XINPUT_STATE xinput_previous_state_{};
		XINPUT_STATE xinput_current_state_{};

		DWORD controller_index_{ XUSER_MAX_COUNT };

		WORD buttons_pressed_this_frame_{};
		WORD buttons_released_this_frame_{};

		std::vector<SDL_Keycode> keys_pressed_this_frame_{};

		InputMappingContext input_mapping_context_{};

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

		bool poll_sdl( )
		{
			SDL_Event e;
			while ( SDL_PollEvent( &e ) )
			{
				switch ( e.type )
				{
				case SDL_QUIT:
					return false;

				case SDL_KEYDOWN:
					if ( std::find( keys_pressed_this_frame_.begin( ), keys_pressed_this_frame_.end( ), e.key.keysym.sym ) == keys_pressed_this_frame_.end( ) )
					{
						keys_pressed_this_frame_.push_back( e.key.keysym.sym );
					}
					break;

				case SDL_KEYUP:
					std::erase( keys_pressed_this_frame_, e.key.keysym.sym );
					input_mapping_context_.dispatch( e.key.keysym.sym, false, TriggerEvent::Released );
					break;

				}

				// Process events for ImGui
				UI_CONTROLLER.process_input( e );
			}

			// Dispatch all keys pressed every frame
			for ( auto key : keys_pressed_this_frame_ )
			{
				input_mapping_context_.dispatch( key, true, TriggerEvent::Pressed );
			}

			return true;
		}

		void update_xinput( )
		{
			// Save to the previous state and reset the current state
			CopyMemory( &xinput_previous_state_, &xinput_current_state_, sizeof( XINPUT_STATE ) );
			ZeroMemory( &xinput_current_state_, sizeof( XINPUT_STATE ) );

			XInputGetState( controller_index_, &xinput_current_state_ );
			clear_deadzone( xinput_current_state_ );

			// XOR between previous and current to get differences
			WORD buttonChanges = xinput_current_state_.Gamepad.wButtons ^ xinput_previous_state_.Gamepad.wButtons;

			// AND between changes and current to get the buttons pressed this frame
			buttons_pressed_this_frame_ = buttonChanges & xinput_current_state_.Gamepad.wButtons;
			// AND between changes and NOT current to get the buttons release this frame
			buttons_released_this_frame_ = buttonChanges & ( ~xinput_current_state_.Gamepad.wButtons );
		}

		void poll_xinput( ) const
		{
			if ( buttons_pressed_this_frame_ )
			{
				input_mapping_context_.dispatch( buttons_pressed_this_frame_, true, TriggerEvent::Pressed );
			}
			if ( buttons_released_this_frame_ )
			{
				input_mapping_context_.dispatch( buttons_released_this_frame_, false, TriggerEvent::Released );
			}
			if ( xinput_current_state_.Gamepad.wButtons )
			{
				// for every bit, we check if set and dispatch the signal
				for ( WORD button = 1; button != 0; button <<= 1 )
				{
					if ( xinput_current_state_.Gamepad.wButtons & button )
					{
						input_mapping_context_.dispatch( button, true, TriggerEvent::Pressed );
					}
				}
			}
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

	InputMappingContext& InputSystem::get_input_mapping_context( )
	{
		return impl_ptr_->get_input_mapping_context();
	}

}
