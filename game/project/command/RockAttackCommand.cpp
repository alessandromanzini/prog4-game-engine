#include "RockAttackCommand.h"

#include <component/CollisionsComponent.h>
#include <component/RockComponent.h>
#include <core/UID.h>
#include <framework/GameObject.h>
#include <framework/component/physics/BoxColliderComponent.h>
#include <registration/tags.h>


namespace game
{
    RockAttackCommand::RockAttackCommand( engine::GameObject& owner, engine::GameObject& socket )
        : AttackCommand( owner, socket, { ROCK_OFFSET_, 0.f } )
    {
    }


    void RockAttackCommand::create_projectile( engine::GameObject& projectile, glm::vec2 direction ) const
    {
        projectile.set_tag( engine::UID( ObjectTags::ROCK ) );
        projectile.add_component<RockComponent>( direction, ROCK_SPEED_ );
        projectile.add_component<engine::BoxColliderComponent>( glm::vec2{ 30.f, 32.f }, glm::vec2{ -15.f, -16.f } );
        projectile.add_component<CollisionsComponent>( );
    }

}
