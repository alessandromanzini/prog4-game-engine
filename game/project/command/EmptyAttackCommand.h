#ifndef EMPTYATTACKCOMMAND_H
#define EMPTYATTACKCOMMAND_H

#include "AttackCommand.h"


namespace game
{
    class EmptyAttackCommand final : public AttackCommand
    {
    public:
        explicit EmptyAttackCommand( engine::GameObject& owner );

    private:
        void create_projectile( engine::GameObject& projectile, glm::vec2 direction ) const override;

    };

}


#endif //!EMPTYATTACKCOMMAND_H
