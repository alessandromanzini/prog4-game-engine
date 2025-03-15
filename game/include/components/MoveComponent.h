#ifndef MOVECOMPONENT_H
#define MOVECOMPONENT_H

// +--------------------------------+
// | Project Headers				|
// +--------------------------------+
#include "components/Component.h"

#include <glm.hpp>

#include <memory>

namespace engine
{
	class Command;
}

namespace game
{
	class MoveComponent final : public engine::Component
	{
	public:
		MoveComponent( engine::GameObjectView& owner, float movementSpeed, bool gamepad );
		~MoveComponent( ) noexcept override;

		void update( ) override;

	private:
		std::unique_ptr<engine::Command> move_command_ptr_{};

		const float movement_speed_{};
		glm::vec2 direction_{};

		void move( glm::vec2 direction, bool );

	};

}

#endif // !MOVECOMPONENT_H
