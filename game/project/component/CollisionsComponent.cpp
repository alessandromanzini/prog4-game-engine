#include "CollisionsComponent.h"

#include <component/BubbleComponent.h>
#include <framework/GameObject.h>
#include <framework/component/physics/BoxColliderComponent.h>
#include <framework/component/physics/PhysicsComponent.h>

#include <registration/tags.h>

using engine::UID;


namespace game
{
    CollisionsComponent::CollisionsComponent( engine::GameObject& owner )
        : Component{ owner }
    {
        // TODO: register hooks for all colliders in the owner not just the first one
        // Register hooks
        if ( const auto component = get_owner( ).get_component<engine::BoxColliderComponent>( );
            component.has_value( ) )
        {
            colliders_hooks_registered_ = true;
            component.value( ).on_begin_overlap.bind( this, &CollisionsComponent::owner_collider_begin_overlap );
        }

        // Register overlap handlers
        overlap_handlers_.insert( { { UID( ObjectTags::ALLY ), UID( ObjectTags::BUBBLE ) }, handle_ally_bubble_overlap } );
        overlap_handlers_.insert( { { UID( ObjectTags::ENEMY ), UID( ObjectTags::BUBBLE ) }, handle_enemy_bubble_overlap } );
        overlap_handlers_.insert( { { UID( ObjectTags::ALLY ), UID( ObjectTags::ENEMY ) }, handle_ally_enemy_overlap } );

        overlap_handlers_.insert( { { UID( ObjectTags::BUBBLE ), UID( ObjectTags::BUBBLE ) }, handle_bubble_bounce } );
        overlap_handlers_.insert( { { UID( ObjectTags::BUBBLE ), engine::NULL_UID }, handle_bubble_bounce } );

        overlap_handlers_.insert( { { UID( ObjectTags::ALLY ), UID( ObjectTags::ALLY ) }, do_nothing } );
        overlap_handlers_.insert( { { UID( ObjectTags::ENEMY ), UID( ObjectTags::ENEMY ) }, do_nothing } );
    }


    CollisionsComponent::~CollisionsComponent( ) noexcept
    {
        if ( colliders_hooks_registered_ )
        {
            colliders_hooks_registered_ = false;
            // TODO: make sure unregistering works on program shutdown
            //get_owner( ).get_component<engine::BoxColliderComponent>( ).value( ).on_begin_overlap.unbind( this );
        }
    }


    void CollisionsComponent::owner_collider_begin_overlap( engine::ColliderComponent& self, engine::ColliderComponent& other,
                                                            const engine::CollisionInfo& info )
    {
        const handlers_pair_t pair{ self.get_owner( ).get_tag( ), other.get_owner( ).get_tag( ) };
        const auto it = overlap_handlers_.find( pair );
        if ( it != overlap_handlers_.end( ) )
        {
            it->second( self, other, info );
        }
        else
        {
            handle_default_overlap( self, other, info );
        }
    }


    void CollisionsComponent::handle_default_overlap( engine::ColliderComponent& self, engine::ColliderComponent& other,
                                                      const engine::CollisionInfo& info ) const
    {
        // If collision is with the roof, ignore it
        if ( dot( info.normal, glm::vec2{ 0.f, 1.f } ) == 1.f )
        {
            return;
        }

        self.get_owner( ).get_transform_operator( ).translate( info.normal * ( info.depth + get_overlap_jitter( info.normal ) ) );
        self.clear_overlap( other );

        // If collision is with the ground, reset the velocity to zero
        if ( const auto physics = get_physics_component( self.get_owner( ) ) )
        {
            // Reset the velocity to zero when a collision occurs
            const auto velocity = physics->get_velocity( );
            if ( const float collDot = dot( -info.normal, velocity ); collDot > .5f )
            {
                physics->add_force( info.normal * collDot, false );
            }
        }
    }


    void CollisionsComponent::handle_ally_bubble_overlap( engine::ColliderComponent& self, engine::ColliderComponent& other,
                                                          const engine::CollisionInfo& info )
    {
        // If collision is vertical, bounce off
        if ( dot( info.normal, glm::vec2{ 0.f, 1.f } ) == 1.f )
        {
            if ( const auto physics = get_physics_component( self.get_owner( ) ) )
            {
                physics->add_force( { 0.f, -physics->get_velocity( ).y * 1.9f } );
            }
        }
        else
        {
            // else push the bubble away
            other.get_owner( ).get_transform_operator( ).translate( info.normal * info.depth );
            self.clear_overlap( other );
        }
    }


    void CollisionsComponent::handle_enemy_bubble_overlap( engine::ColliderComponent&, engine::ColliderComponent&,
                                                           const engine::CollisionInfo& ) { }


    void CollisionsComponent::handle_ally_enemy_overlap( engine::ColliderComponent&, engine::ColliderComponent&,
                                                         const engine::CollisionInfo& ) { }


    void CollisionsComponent::handle_bubble_bounce( engine::ColliderComponent& self, engine::ColliderComponent&,
                                                    const engine::CollisionInfo& info )
    {
        if ( const auto bubble = get_bubble_component( self.get_owner( ) ); bubble != nullptr )
        {
            bubble->bounce( info.normal, info.depth );
        }
    }


    void CollisionsComponent::do_nothing( engine::ColliderComponent&, engine::ColliderComponent&,
                                          const engine::CollisionInfo& ) { }


    engine::PhysicsComponent* CollisionsComponent::get_physics_component( const engine::GameObject& object )
    {
        if ( const auto component = object.get_component<engine::PhysicsComponent>( );
            component.has_value( ) )
        {
            return &component.value( );
        }
        return nullptr;
    }


    BubbleComponent* CollisionsComponent::get_bubble_component( const engine::GameObject& object )
    {
        if ( const auto component = object.get_component<BubbleComponent>( );
            component.has_value( ) )
        {
            return &component.value( );
        }
        return nullptr;
    }


    float CollisionsComponent::get_overlap_jitter( const glm::vec2 normal )
    {
        if ( normal.y == 0.f )
        {
            return 2.1f;
        }
        return 0.5f;
    }

}
