#ifndef ROCKATTACKCOMMAND_H
#define ROCKATTACKCOMMAND_H

#include "AttackCommand.h"

#include <vec2.hpp>


namespace engine
{
    class GameObject;
}

namespace game
{
    class RockAttackCommand final : public AttackCommand
    {
    public:
        explicit RockAttackCommand( engine::GameObject& owner, engine::GameObject& socket );

    private:
        static constexpr float ROCK_SPEED_{ 80.f };
        static constexpr float ROCK_OFFSET_{ 25.f };

        void create_projectile( engine::GameObject& projectile, glm::vec2 direction ) const override;

    };

}


#endif //!ROCKATTACKCOMMAND_H
