#include "CollisionsComponent.h"

#include <component/BubbleComponent.h>
#include <framework/GameObject.h>
#include <framework/Scene.h>
#include <framework/component/physics/BoxColliderComponent.h>
#include <framework/component/physics/PhysicsComponent.h>
#include <registration/object_initializers.h>

#include <registration/tags.h>

#include "BubbleCaptureComponent.h"
#include "CharacterComponent.h"
#include "FruitComponent.h"
#include "GameOverDelegateComponent.h"
#include "RockComponent.h"
#include "ScoreDelegateComponent.h"

using engine::UID;


namespace game
{
    CollisionsComponent::CollisionsComponent( engine::GameObject& owner )
        : Component{ owner }
    {
        // Register hooks
        if ( const auto component = get_owner( ).get_component<engine::BoxColliderComponent>( );
            component.has_value( ) )
        {
            colliders_hooks_registered_ = true;
            component.value( ).on_begin_overlap.bind( this, &CollisionsComponent::owner_collider_begin_overlap );
        }

        // Register overlap handlers
        overlap_handlers_.insert( { { UID( ObjectTags::ALLY ), UID( ObjectTags::ALLY ) }, handle_ally_ally_overlap } );
        overlap_handlers_.insert( { { UID( ObjectTags::ALLY ), UID( ObjectTags::BUBBLE ) }, handle_ally_bubble_overlap } );
        overlap_handlers_.insert( { { UID( ObjectTags::ALLY ), UID( ObjectTags::ROCK ) }, handle_ally_death } );
        overlap_handlers_.insert( { { UID( ObjectTags::ALLY ), UID( ObjectTags::ENEMY ) }, handle_ally_enemy_overlap } );
        overlap_handlers_.insert( { { UID( ObjectTags::ALLY ), UID( ObjectTags::FRUIT ) }, handle_ally_fruit_overlap } );

        overlap_handlers_.insert( { { UID( ObjectTags::ENEMY ), UID( ObjectTags::BUBBLE ) }, handle_enemy_bubble_overlap } );
        overlap_handlers_.insert( { { UID( ObjectTags::ENEMY ), engine::NULL_UID }, handle_default_overlap } );
        overlap_handlers_.insert( { { UID( ObjectTags::ENEMY ), UID( ObjectTags::ROCK ) }, do_nothing } );
        overlap_handlers_.insert( { { UID( ObjectTags::ENEMY ), UID( ObjectTags::ALLY ) }, do_nothing } );
        overlap_handlers_.insert( { { UID( ObjectTags::ENEMY ), UID( ObjectTags::FRUIT ) }, do_nothing } );

        overlap_handlers_.insert( { { UID( ObjectTags::BUBBLE ), UID( ObjectTags::BUBBLE ) }, handle_bubble_bounce } );
        overlap_handlers_.insert( { { UID( ObjectTags::BUBBLE ), UID( ObjectTags::ROCK ) }, handle_bubble_destroy } );
        overlap_handlers_.insert( { { UID( ObjectTags::BUBBLE ), UID( ObjectTags::PLATFORM ) }, handle_bubble_bounce } );
        overlap_handlers_.insert( { { UID( ObjectTags::BUBBLE ), engine::NULL_UID }, handle_bubble_bounce } );

        overlap_handlers_.insert( { { UID( ObjectTags::ROCK ), UID( ObjectTags::ENEMY ) }, do_nothing } );
        overlap_handlers_.insert( { { UID( ObjectTags::ROCK ), UID( ObjectTags::ALLY ) }, handle_rock_destroy } );
        overlap_handlers_.insert( { { UID( ObjectTags::ROCK ), UID( ObjectTags::BUBBLE ) }, do_nothing } );
        overlap_handlers_.insert( { { UID( ObjectTags::ROCK ), UID( ObjectTags::ROCK ) }, handle_rock_destroy } );
        overlap_handlers_.insert( { { UID( ObjectTags::ROCK ), UID( ObjectTags::PLATFORM ) }, handle_rock_destroy } );
        overlap_handlers_.insert( { { UID( ObjectTags::ROCK ), engine::NULL_UID }, handle_rock_destroy } );

        overlap_handlers_.insert( { { UID( ObjectTags::FRUIT ), UID( ObjectTags::PLATFORM ) }, handle_fruit_bounce } );
        overlap_handlers_.insert( { { UID( ObjectTags::FRUIT ), engine::NULL_UID }, handle_fruit_bounce } );

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
        if ( const auto it = overlap_handlers_.find( pair ); it != overlap_handlers_.end( ) )
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


    void CollisionsComponent::handle_ally_ally_overlap( engine::ColliderComponent& self, engine::ColliderComponent& other,
                                                        const engine::CollisionInfo& info )
    {
        if ( dot( info.normal, glm::vec2{ 0.f, 1.f } ) == 1.f )
        {
            return;
        }
        handle_default_overlap( self, other, info );
    }


    void CollisionsComponent::handle_ally_bubble_overlap( engine::ColliderComponent& self, engine::ColliderComponent& other,
                                                          const engine::CollisionInfo& info )
    {
        // If the bubble has captured a target, create a fruit object
        if ( const auto bubble = get_component<BubbleComponent>( other.get_owner( ) );
            bubble && bubble->has_captured_target( ) )
        {
            auto& fruit        = bubble->get_owner( ).get_owning_scene( ).create_object( );

            const auto capture = bubble->get_captured_target( );
            if ( not capture )
            {
                return;
            }

            if ( const auto gameover = get_component<GameOverDelegateComponent>( capture->get_owner( ) ); gameover != nullptr )
            {
                gameover->signal_gameover( );
                bubble->get_owner( ).mark_for_deletion( );
            }
            else
            {
                create_fruit( fruit, capture->get_fruit_texture_path( ),
                              capture->get_fruit_value( ),
                              bubble->get_owner( ).get_world_transform( ).get_position( ),
                              self.get_owner( ).get_world_transform( ).get_position( ) );
            }

            bubble->get_owner( ).mark_for_deletion( );
        }
        // If collision is vertical, bounce off
        else if ( dot( info.normal, glm::vec2{ 0.f, -1.f } ) == 1.f )
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


    void CollisionsComponent::handle_ally_fruit_overlap( const engine::ColliderComponent& self,
                                                         const engine::ColliderComponent& other,
                                                         const engine::CollisionInfo& )
    {
        if ( const auto fruit = get_component<FruitComponent>( other.get_owner( ) ); fruit && fruit->is_capturable( ) )
        {
            // destroy the fruit object
            other.get_owner( ).mark_for_deletion( );
            if ( const auto score = get_component<ScoreDelegateComponent>( self.get_owner( ) ) )
            {
                score->increase_score( fruit->get_fruit_value( ) );
            }
        }
    }


    void CollisionsComponent::handle_enemy_bubble_overlap( const engine::ColliderComponent& self,
                                                           const engine::ColliderComponent& other,
                                                           const engine::CollisionInfo& )
    {
        if ( const auto bubble = get_component<BubbleComponent>( other.get_owner( ) );
            bubble && not bubble->has_captured_target( ) )
        {
            // If the enemy collides with the bubble, capture it
            bubble->capture( self.get_owner( ) );
        }
    }


    void CollisionsComponent::handle_ally_enemy_overlap( engine::ColliderComponent& self,
                                                         engine::ColliderComponent& other,
                                                         const engine::CollisionInfo& info )
    {
        const auto ally = get_component<CharacterComponent>( self.get_owner( ) );
        const auto enemy = get_component<CharacterComponent>( other.get_owner( ) );
        if ( ( enemy && enemy->is_locked( ) ) || ( ally && ally->is_iframing( ).first ) )
        {
            return;
        }
        handle_ally_death( self, other, info );
    }


    void CollisionsComponent::handle_bubble_bounce( const engine::ColliderComponent& self, engine::ColliderComponent&,
                                                    const engine::CollisionInfo& info )
    {
        if ( const auto bubble = get_component<BubbleComponent>( self.get_owner( ) ); bubble )
        {
            bubble->bounce( info.normal, info.depth );
        }
    }


    void CollisionsComponent::handle_ally_death( engine::ColliderComponent& self, engine::ColliderComponent&,
                                                 const engine::CollisionInfo& )
    {
        if ( const auto gameover = get_component<GameOverDelegateComponent>( self.get_owner( ) ); gameover )
        {
            gameover->signal_gameover( );
            self.get_owner(  ).mark_for_deletion( );
            return;
        }
        if ( const auto score = get_component<ScoreDelegateComponent>( self.get_owner( ) ); score )
        {
            // If the ally collides with the enemy, signal player death
            score->signal_player_death( );
        }
        if ( const auto character = get_component<CharacterComponent>( self.get_owner( ) ); character )
        {
            // If the ally collides with the enemy, reposition the character
            character->reposition( );
        }
    }


    void CollisionsComponent::handle_bubble_destroy( const engine::ColliderComponent& self, engine::ColliderComponent&,
                                                     const engine::CollisionInfo& )
    {
        self.get_owner( ).mark_for_deletion( );
    }


    void CollisionsComponent::handle_rock_destroy( const engine::ColliderComponent& self, engine::ColliderComponent&,
                                                   const engine::CollisionInfo& )
    {
        if ( const auto rock = get_component<RockComponent>( self.get_owner( ) ); rock != nullptr )
        {
            rock->destroy( );
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
