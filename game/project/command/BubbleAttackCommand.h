#ifndef BUBBLEATTACKCOMMAND_H
#define BUBBLEATTACKCOMMAND_H

#include "AttackCommand.h"


namespace game
{
    class BubbleAttackCommand final : public AttackCommand
    {
    public:
        explicit BubbleAttackCommand( engine::GameObject& owner, engine::GameObject& socket );

    private:
        static constexpr float BUBBLE_SPEED_{ 100.f };
        static constexpr float BUBBLE_OFFSET_{ 10.f };

        void create_projectile( engine::GameObject& projectile, glm::vec2 direction ) const override;

    };

}


#endif //!BUBBLEATTACKCOMMAND_H
