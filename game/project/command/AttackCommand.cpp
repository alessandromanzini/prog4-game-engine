#include "AttackCommand.h"

#include <component/BubbleComponent.h>
#include <component/CollisionsComponent.h>
#include <framework/GameObject.h>
#include <framework/Scene.h>
#include <framework/component/physics/BoxColliderComponent.h>
#include <registration/tags.h>


namespace game
{
    AttackCommand::AttackCommand( engine::GameObject& owner, engine::GameObject& projectileSocket, const glm::vec2 offset )
        : GameObjectCommand( owner )
        , projectile_offset_{ offset }
        , projectile_socket_ref_{ projectileSocket } { }


    void AttackCommand::execute( )
    {
        engine::GameObject& projectile =  get_object( ).get_owning_scene( ).create_object( );
        projectile.set_world_transform( projectile_socket_ref_.get_world_transform( ).translate( attack_direction_ * projectile_offset_ ) );
        create_projectile( projectile, attack_direction_ );
    }


    void AttackCommand::set_attack_direction( const glm::vec2 direction )
    {
        attack_direction_ = normalize( direction );
    }

}
