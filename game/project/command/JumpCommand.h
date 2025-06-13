#ifndef JUMPCOMMAND_H
#define JUMPCOMMAND_H

#include <framework/binding/command/GameObjectCommand.h>

#include <chrono>


namespace engine
{
    class PhysicsComponent;
}

namespace game
{
    class JumpCommand final : public engine::GameObjectCommand
    {
    public:
        explicit JumpCommand( engine::GameObject& owner, float force );
        void execute( ) override;
        [[nodiscard]] bool can_jump( ) const;

    private:
        static constexpr float JUMP_TIME_DELAY_{ .25f };
        const float force_{ 0.f };
        std::chrono::time_point<std::chrono::system_clock> jump_time_{};

        engine::PhysicsComponent* physics_body_ptr_{ nullptr };

    };

}


#endif //!JUMPCOMMAND_H
