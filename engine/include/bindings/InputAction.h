#ifndef INPUTACTION_H
#define INPUTACTION_H

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include "BindingTypes.h"
#include "UID.h"

// +--------------------------------+
// | GLM HEADERS					|
// +--------------------------------+
#include <glm.hpp>

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <variant>
#include <functional>

namespace engine
{
	// +--------------------------------+
	// | INPUT ACTION					|
	// +--------------------------------+
	template <typename value_t>
		requires std::same_as<value_t, bool> or std::same_as<value_t, float> //or std::same_as<value_t, glm::vec2>
	struct InputAction final
	{
		typedef value_t value_type;

		binding::modifier_bitset_t modifiers{};
	};

	// +--------------------------------+
	// | VARIANTS						|
	// +--------------------------------+
	using input_action_value_variant_t = std::variant<bool, float /*, glm::vec2 */>;
	using input_action_variant_t = std::variant<InputAction<bool>, InputAction<float> /*, InputAction<glm::vec2> */>;

	// +--------------------------------+
	// | INPUT ACTION BINDING			|
	// +--------------------------------+
	struct InputActionBinding final
	{
		UID uid{ 0 };
		input_action_variant_t input_action{};
	};

	// +--------------------------------+
	// | ACTION CONTEXT					|
	// +--------------------------------+
	struct InputActionContext final
	{
		UID uid{ 0 };
		input_action_value_variant_t value{};
		binding::trigger_bitset_t triggers{};
	};

}

#endif // !INPUTACTION_H
