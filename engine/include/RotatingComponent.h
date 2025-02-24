#ifndef ROTATINGCOMPONENT_H
#define ROTATINGCOMPONENT_H

#include "GameTime.h"
#include "BaseComponent.h"

namespace engine
{
	class RotatingComponent final : public BaseComponent
	{
	public:
		RotatingComponent( GameObjectView& owner, float rotationSpeed );

		void update( ) override;

	private:
		float rotation_speed_{};

	};
}

#endif // !ROTATINGCOMPONENT_H
