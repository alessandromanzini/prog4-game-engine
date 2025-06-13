#include "BubbleAttackCommand.h"

#include <component/BubbleComponent.h>
#include <component/CollisionsComponent.h>
#include <core/UID.h>
#include <framework/GameObject.h>
#include <framework/component/physics/BoxColliderComponent.h>
#include <registration/tags.h>


namespace game
{
    BubbleAttackCommand::BubbleAttackCommand( engine::GameObject& owner, engine::GameObject& socket )
        : AttackCommand( owner, socket, { BUBBLE_OFFSET_, 0.f } )
    {
    }


    void BubbleAttackCommand::create_projectile( engine::GameObject& projectile, glm::vec2 direction ) const
    {
        projectile.set_tag( engine::UID( ObjectTags::BUBBLE ) );
        projectile.add_component<BubbleComponent>( direction, BUBBLE_SPEED_ );
        projectile.add_component<engine::BoxColliderComponent>( glm::vec2{ 28.f, 32.f }, glm::vec2{ -14.f, -16.f } );
        projectile.add_component<CollisionsComponent>( );
    }

}
