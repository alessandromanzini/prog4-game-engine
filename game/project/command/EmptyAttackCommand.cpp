#include "EmptyAttackCommand.h"

#include <framework/GameObject.h>


namespace game
{
    EmptyAttackCommand::EmptyAttackCommand( engine::GameObject& owner ) : AttackCommand( owner, owner ) { }

    void EmptyAttackCommand::create_projectile( engine::GameObject& projectile, glm::vec2 ) const
    {
        projectile.mark_for_deletion(  );
    }

}
