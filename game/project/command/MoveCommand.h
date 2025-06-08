#ifndef MOVECOMMAND_H
#define MOVECOMMAND_H

#include <framework/binding/command/GameObjectCommand.h>


namespace engine
{
    class PhysicsComponent;
}

namespace game
{
    class MoveCommand final : public engine::GameObjectCommand
    {
    public:
        explicit MoveCommand( engine::GameObject& owner, float speed );
        void execute( ) override;

        void set_movement( float movement );

        [[nodiscard]] bool is_facing_right( ) const;

    private:
        const float speed_{ 0.f };

        engine::PhysicsComponent* physics_body_ptr_{ nullptr };

        float movement_{ 0.f };
        bool is_facing_right_{ false };

    };

}


#endif //!MOVECOMMAND_H
