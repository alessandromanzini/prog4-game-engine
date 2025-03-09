#ifndef IABMOVE_H
#define IABMOVE_H

#include "bindings/InputActionBuilder.h"

namespace game
{
	class IABMove final : public engine::InputActionBuilder<IABMove, bool>
	{
	public:
		void build( engine::InputAction<bool>& action ) override
		{
			action.triggers.set( static_cast<engine::trigger_mask_t>( engine::TriggerEvent::Pressed ) );
		}
	};

}

#endif // !IABMOVE_H
