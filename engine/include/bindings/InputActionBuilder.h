#ifndef INPUTACTIONBUILDER_H
#define INPUTACTIONBUILDER_H

#include "InputAction.h"
#include "type_utility.hpp"

namespace engine
{
	template <typename derived_t, typename action_value_t>
	struct InputActionBuilder
	{
		typedef action_value_t value_type;

		static constexpr uint64_t type_hash = type_utility::type_hash<derived_t>( );

		virtual void build( InputAction<action_value_t>& action ) = 0;

	};

	// +--------------------------------+
	// | CONCEPTS						|
	// +--------------------------------+
	// TODO: Fix concept
	template <typename builder_t>
	concept DerivedInputActionBuilder = requires {
		typename builder_t::value_type;
	};

}

#endif // !INPUTACTIONBUILDER_H
