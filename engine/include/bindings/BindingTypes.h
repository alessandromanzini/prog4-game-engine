#ifndef BINDINGTYPES_H
#define BINDINGTYPES_H

#include "SDL_keycode.h"
#include "SDL_gamecontroller.h"

#include <bitset>
#include <cassert>

namespace engine::binding
{
	// +--------------------------------+
	// | KEY/BTN TYPES					|
	// +--------------------------------+
	typedef SDL_KeyCode key_t;
	typedef SDL_GameControllerButton btn_t;

	// +--------------------------------+
	// | ENUMERATIONS					|
	// +--------------------------------+
	typedef uint8_t trigger_mask_t;
	enum class TriggerEvent : trigger_mask_t
	{
		TRIGGERED	= 0x1,
		PRESSED		= 0x2,
		RELEASED	= 0x3,

		ALL			= 0xF
	};

	typedef uint8_t modifier_mask_t;
	enum class ModifierType : modifier_mask_t
	{
		NEGATE		= 0x1,
		SWIZZLE		= 0x2,

		ALL			= 0xF
	};

	// +--------------------------------+
	// | BITSET TYPES					|
	// +--------------------------------+
	using trigger_bitset_t = std::bitset<sizeof( binding::trigger_mask_t ) * 8>;
	using modifier_bitset_t = std::bitset<sizeof( binding::modifier_mask_t ) * 8>;

	// +--------------------------------+
	// | CASTS							|
	// +--------------------------------+
	inline constexpr [[nodiscard]] uint32_t mask_to_seq( uint32_t mask )
	{
		assert( mask != 0 && "Mask cannot be 0!" );

		uint32_t pos = 0;
		while ( ( mask & 1 ) == 0 )
		{
			mask >>= 1;
			pos++;
		}
		return pos;
	}
	
	// Casts the enum to the underlying type
	inline constexpr [[nodiscard]] uint32_t seq_mask_cast( TriggerEvent trigger ) noexcept
	{
		return mask_to_seq( static_cast<uint32_t>( trigger ) );
	}

	// Casts the enum to the underlying type
	inline constexpr [[nodiscard]] uint32_t seq_mask_cast( ModifierType modifier ) noexcept
	{
		return mask_to_seq( static_cast<uint32_t>( modifier ) );
	}

	// Casts the enum to the bitset representation
	inline constexpr [[nodiscard]] trigger_bitset_t bitset_cast( TriggerEvent trigger ) noexcept
	{
		return static_cast<trigger_mask_t>( trigger );
	}

	// Casts the enum to the bitset representation
	inline constexpr [[nodiscard]] modifier_bitset_t bitset_cast( ModifierType modifier ) noexcept
	{
		return static_cast<modifier_mask_t>( modifier );
	}
}

#endif // !BINDINGTYPES_H
