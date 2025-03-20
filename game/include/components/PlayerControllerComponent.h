#ifndef PLAYERCONTROLLERCOMPONENT_H
#define PLAYERCONTROLLERCOMPONENT_H

// +------------------------------------+
// | PROJECT HEADERS					|
// +------------------------------------+
#include "components/Component.h"
#include "singletons/InputSystem.h"

// +------------------------------------+
// | STANDARD HEADERS					|
// +------------------------------------+
#include <memory>


namespace game
{
    class AttackCommand;
    class ScoreCommand;
    class PlayerControllerComponent final : public engine::Component
    {
    public:
        PlayerControllerComponent( engine::GameObjectView& owner, bool gamepad );
        ~PlayerControllerComponent( ) noexcept override;

    private:
        std::unique_ptr<engine::Command> attack_command_{};
        std::unique_ptr<engine::Command> score_up_command_{};
        std::unique_ptr<engine::Command> score_great_up_command_{};

    };

}


#endif // !PLAYERCONTROLLERCOMPONENT_H
