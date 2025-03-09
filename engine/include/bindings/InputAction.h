#ifndef INPUTACTION_H
#define INPUTACTION_H

// +--------------------------------+
// | GLM HEADERS					|
// +--------------------------------+
#include <glm.hpp>

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <bitset>
#include <variant>
#include <functional>

namespace engine
{
	// +--------------------------------+
	// | ENUMERATIONS					|
	// +--------------------------------+
	typedef uint16_t trigger_mask_t;
	enum class TriggerEvent : trigger_mask_t
	{
		Pressed		= 0x0001,
		Released	= 0x0002
	};

	typedef uint16_t modifier_mask_t;
	enum class ModifierType : modifier_mask_t
	{
		Negate		= 0x0001,
		SwizzleXY	= 0x0002,
		SwizzleYX	= 0x0004
	};

	// +--------------------------------+
	// | VARIANTS						|
	// +--------------------------------+
	using action_value_variant_t = std::variant<bool, float /*, glm::vec2*/ >;
	using command_variant_t = std::variant<std::function<void( bool )>, std::function<void( float )> /*, std::function<void( glm::vec2 )*/ >;

	// +--------------------------------+
	// | INPUT ACTION					|
	// +--------------------------------+
	template <typename value_t>
		requires std::same_as<value_t, bool> or std::same_as<value_t, float> //or std::same_as<value_t, glm::vec2>
	struct InputAction final
	{
		std::bitset<sizeof( trigger_mask_t ) * 8> triggers{};
		std::bitset<sizeof( modifier_mask_t ) * 8> modifiers{};
	};

}

#endif // !INPUTACTION_H
