#ifndef ATTACKCOMMAND_H
#define ATTACKCOMMAND_H

#include <framework/binding/command/GameObjectCommand.h>

#include <vec2.hpp>


namespace game
{
    class AttackCommand : public engine::GameObjectCommand
    {
    public:
        explicit AttackCommand( engine::GameObject& owner, engine::GameObject& projectileSocket, glm::vec2 offset = { 0.f, 0.f } );
        void execute( ) override;
        void set_attack_direction( glm::vec2 direction );

    protected:
        virtual void create_projectile( engine::GameObject& projectile, glm::vec2 direction ) const = 0;

    private:
        const glm::vec2 projectile_offset_{ 0.f };

        engine::GameObject& projectile_socket_ref_;
        glm::vec2 attack_direction_{ 0.f };

    };

}


#endif //!ATTACKCOMMAND_H
