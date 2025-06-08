#ifndef ATTACKCOMMAND_H
#define ATTACKCOMMAND_H

#include <framework/binding/command/GameObjectCommand.h>

#include <vec2.hpp>


namespace game
{
    class AttackCommand final : public engine::GameObjectCommand
    {
    public:
        explicit AttackCommand( engine::GameObject& owner, engine::GameObject& projectileSocket, float speed, float offset = 0.f );
        void execute( ) override;
        void set_attack_direction( glm::vec2 direction );

    private:
        const float projectile_speed_{};
        const float projectile_offset_{};
        engine::GameObject& projectile_socket_ref_;

        glm::vec2 attack_direction_{ 0.f };

    };

}


#endif //!ATTACKCOMMAND_H
