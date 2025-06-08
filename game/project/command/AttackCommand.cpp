#include "AttackCommand.h"

#include <component/BubbleComponent.h>
#include <component/CollisionsComponent.h>
#include <framework/GameObject.h>
#include <framework/Scene.h>
#include <framework/component/physics/BoxColliderComponent.h>
#include <registration/tags.h>


namespace game
{
    AttackCommand::AttackCommand( engine::GameObject& owner, engine::GameObject& projectileSocket, const float speed,
                                  const float offset )
        : GameObjectCommand( owner )
        , projectile_speed_{ speed }
        , projectile_offset_{ offset }
        , projectile_socket_ref_{ projectileSocket } { }


    void AttackCommand::execute( )
    {
        engine::Scene& scene = get_object( ).get_owning_scene( );

        engine::GameObject& projectile = scene.create_object( );
        projectile.set_tag( engine::UID( ObjectTags::BUBBLE ) );
        projectile.set_world_transform(
            projectile_socket_ref_.get_world_transform( ).translate( attack_direction_ * projectile_offset_ ) );
        projectile.add_component<BubbleComponent>( attack_direction_, projectile_speed_ );

        projectile.add_component<engine::BoxColliderComponent>( glm::vec2{ 28.f, 32.f }, glm::vec2{ -14.f, -16.f } );
        projectile.add_component<CollisionsComponent>( );
    }


    void AttackCommand::set_attack_direction( const glm::vec2 direction )
    {
        attack_direction_ = normalize( direction );
    }

}
