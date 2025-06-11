#include "CollisionsComponent.h"

#include <component/BubbleComponent.h>
#include <framework/GameObject.h>
#include <framework/Scene.h>
#include <framework/component/TextureComponent.h>
#include <framework/component/physics/BoxColliderComponent.h>
#include <framework/component/physics/PhysicsComponent.h>
#include <registration/object_initializers.h>

#include <registration/tags.h>

#include "BubbleCaptureComponent.h"
#include "FruitComponent.h"

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
        overlap_handlers_.insert( { { UID( ObjectTags::ALLY ), UID( ObjectTags::ENEMY ) }, handle_ally_enemy_overlap } );
        overlap_handlers_.insert( { { UID( ObjectTags::ALLY ), UID( ObjectTags::FRUIT ) }, handle_ally_fruit_overlap } );

        overlap_handlers_.insert( { { UID( ObjectTags::ENEMY ), UID( ObjectTags::BUBBLE ) }, handle_enemy_bubble_overlap } );

        overlap_handlers_.insert( { { UID( ObjectTags::BUBBLE ), UID( ObjectTags::BUBBLE ) }, handle_bubble_bounce } );
        overlap_handlers_.insert( { { UID( ObjectTags::BUBBLE ), UID( ObjectTags::PLATFORM ) }, handle_bubble_bounce } );
        overlap_handlers_.insert( { { UID( ObjectTags::BUBBLE ), engine::NULL_UID }, handle_bubble_bounce } );

        overlap_handlers_.insert( { { UID( ObjectTags::FRUIT ), UID( ObjectTags::PLATFORM ) }, handle_fruit_bounce } );
        overlap_handlers_.insert( { { UID( ObjectTags::FRUIT ), engine::NULL_UID }, handle_fruit_bounce } );

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
                                                      const engine::CollisionInfo& info )
    {
        // If collision is with the platform roof, ignore it
        if ( dot( info.normal, glm::vec2{ 0.f, 1.f } ) == 1.f
             && other.get_owner( ).get_tag( ) == UID( ObjectTags::PLATFORM ) )
        {
            return;
        }

        self.get_owner( ).get_transform_operator( ).translate( info.normal * ( info.depth + get_overlap_jitter( info.normal ) ) );
        self.clear_overlap( other );

        // If collision is with the ground, reset the velocity to zero
        if ( const auto physics = get_component<engine::PhysicsComponent>( self.get_owner( ) ) )
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
        // If the bubble has captured a target, create a fruit object
        if ( const auto bubble = get_component<BubbleComponent>( other.get_owner( ) );
            bubble && bubble->has_captured_target( ) )
        {
            auto& fruit  = bubble->get_owner( ).get_owning_scene( ).create_object( );
            const auto capture = bubble->get_captured_target( );
            create_fruit( fruit, capture->get_fruit_texture_path( ),
                          capture->get_fruit_value( ), bubble->get_owner( ).get_world_transform( ).get_position( ) );
            bubble->get_owner( ).mark_for_deletion( );
            return;
        }

        // If collision is vertical, bounce off
        if ( dot( info.normal, glm::vec2{ 0.f, -1.f } ) == 1.f )
        {
            constexpr float FLAT_BOUNCE_REBOUND{ -200.f };
            constexpr float RELATIVE_BOUNCE_REBOUND{ 1.25f };
            if ( const auto physics = get_component<engine::PhysicsComponent>( self.get_owner( ) ) )
            {
                const float rebound = FLAT_BOUNCE_REBOUND - physics->get_velocity( ).y * RELATIVE_BOUNCE_REBOUND;
                physics->add_force( { 0.f, rebound } );
                other.get_owner( ).get_transform_operator( ).translate( { 0.f, 5.f } );
            }
        }
        else
        {
            // else push the bubble away
            other.get_owner( ).get_transform_operator( ).translate( info.normal * info.depth );
            self.clear_overlap( other );
        }
    }


    void CollisionsComponent::handle_ally_fruit_overlap( engine::ColliderComponent&, engine::ColliderComponent& other,
                                                         const engine::CollisionInfo& )
    {
        if ( const auto fruit = get_component<FruitComponent>( other.get_owner( ) ); fruit->is_capturable(  ) )
        {
            // destroy the fruit object
            other.get_owner( ).mark_for_deletion( );

            // add points to the player
            // todo
        }
    }


    void CollisionsComponent::handle_enemy_bubble_overlap( engine::ColliderComponent& self, engine::ColliderComponent& other,
                                                           const engine::CollisionInfo& )
    {
        if ( const auto bubble = get_component<BubbleComponent>( other.get_owner( ) );
            bubble != nullptr && not bubble->has_captured_target( ) )
        {
            // If the enemy collides with the bubble, capture it
            bubble->capture( self.get_owner( ) );
        }
    }


    void CollisionsComponent::handle_ally_enemy_overlap( engine::ColliderComponent&, engine::ColliderComponent& other,
                                                         const engine::CollisionInfo& )
    {
        if ( const auto capture = get_component<BubbleCaptureComponent>( other.get_owner( ) );
            capture != nullptr && capture->is_captured( ) )
        {
            return;
        }
        // todo: ally death logic
    }


    void CollisionsComponent::handle_bubble_bounce( engine::ColliderComponent& self, engine::ColliderComponent&,
                                                    const engine::CollisionInfo& info )
    {
        if ( const auto bubble = get_component<BubbleComponent>( self.get_owner( ) ); bubble != nullptr )
        {
            bubble->bounce( info.normal, info.depth );
        }
    }


    void CollisionsComponent::handle_fruit_bounce( engine::ColliderComponent& self, engine::ColliderComponent& other,
                                                   const engine::CollisionInfo& info )
    {
        if ( const auto fruit = get_component<FruitComponent>( self.get_owner( ) ); fruit != nullptr )
        {
            fruit->bounce( info.normal, info.depth );
            handle_default_overlap( self, other, info );
        }
    }


    void CollisionsComponent::do_nothing( engine::ColliderComponent&, engine::ColliderComponent&,
                                          const engine::CollisionInfo& ) { }


    float CollisionsComponent::get_overlap_jitter( const glm::vec2 normal )
    {
        if ( normal.y == 0.f )
        {
            return 2.1f;
        }
        return 0.5f;
    }

}
