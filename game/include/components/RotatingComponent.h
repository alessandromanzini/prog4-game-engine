#ifndef ROTATINGCOMPONENT_H
#define ROTATINGCOMPONENT_H

// +--------------------------------+
// | Project Headers				|
// +--------------------------------+
#include "singletons/GameTime.h"
#include "components/BaseComponent.h"

namespace game
{
	class RotatingComponent final : public engine::BaseComponent
	{
	public:
		RotatingComponent( engine::GameObjectView& owner, float rotationSpeed );

		void update( ) override;

	private:
		float rotation_speed_{};

	};
}

#endif // !ROTATINGCOMPONENT_H
