#ifndef PLAYERCONTROLLERCOMPONENT_H
#define PLAYERCONTROLLERCOMPONENT_H

#include "singletons/InputSystem.h"
#include "bindings/InputUtils.h"
#include "components/Component.h"

#include <memory>

namespace game
{
	class AttackCommand;
	class ScoreCommand;
	class PlayerControllerComponent : public engine::Component
	{
	public:
		PlayerControllerComponent( engine::GameObjectView& owner, bool gamepad );

	private:
		std::unique_ptr<engine::Command> attack_command_{};
		std::unique_ptr<engine::Command> score_up_command_{};
		std::unique_ptr<engine::Command> score_great_up_command_{};

	};

}

#endif // !PLAYERCONTROLLERCOMPONENT_H
