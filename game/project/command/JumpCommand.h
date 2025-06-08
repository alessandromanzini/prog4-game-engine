#ifndef JUMPCOMMAND_H
#define JUMPCOMMAND_H

#include <framework/binding/command/GameObjectCommand.h>


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

    private:
        const float force_{ 0.f };

        engine::PhysicsComponent* physics_body_ptr_{ nullptr };

    };

}


#endif //!JUMPCOMMAND_H
