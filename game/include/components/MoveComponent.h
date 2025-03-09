#ifndef MOVECOMPONENT_H
#define MOVECOMPONENT_H

// +--------------------------------+
// | Project Headers				|
// +--------------------------------+
#include "components/Component.h"

#include <glm.hpp>

namespace game
{
	class MoveComponent final : public engine::Component
	{
	public:
		MoveComponent( engine::GameObjectView& owner, float movementSpeed, bool gamepad = true );

		void update( ) override;

	private:
		const float movement_speed_{};

		glm::vec2 direction_{};

		void move( glm::vec2 direction, bool );

	};

}

#endif // !MOVECOMPONENT_H
