#include "BubbleComponent.h"

#include <framework/GameObject.h>
#include <singleton/GameTime.h>
#include <singleton/Renderer.h>


namespace game
{
    BubbleComponent::BubbleComponent( owner_t& owner, const glm::vec2 initialDirection, const float bubbleSpeed )
        : Component{ owner }
        , bubble_speed_{ bubbleSpeed }
        , direction_{ initialDirection }
        , spawn_sprite_{ "characters/bub/bubble_spawn_1x6.png", 1u, 6u, 0.15f, 2.f, { -14.f, -16.f }, false }
        , default_sprite_{ "characters/bub/bubble_lifetime_1x3.png", 1u, 3u, 2.f, 2.f, { -14.f, -16.f }, false }
    {
    }


    void BubbleComponent::tick( )
    {
        const float speed{ bubble_speed_ * speed_modifier_ * engine::GAME_TIME.get_delta_time( ) };
        const glm::vec2 direction{ pathing_ ? direction_ : glm::vec2{ 0.f, -1.f }  };
        get_owner( ).get_transform_operator( ).translate( direction * speed );

        manage_lifetime( );
        if ( popped_ )
        {
            get_owner( ).mark_for_deletion( );
            return;
        }

        current_sprite_ptr_->tick( );
    }


    void BubbleComponent::render( ) const
    {
        engine::RENDERER.set_z_index( 1 );
        current_sprite_ptr_->render( get_owner( ).get_world_transform( ).get_position( ) );
        engine::RENDERER.set_z_index( 0 );
    }


    void BubbleComponent::bounce( const glm::vec2 normal, const float depth )
    {
        if ( dot( normal, glm::vec2{ 0.f, 1.f } ) == 1.f )
        {
            get_owner(  ).get_transform_operator(  ).translate( normal * ( depth + .1f ) );
            pathing_ = true;
        }
        else
        {
            direction_ = normal;
        }
    }


    void BubbleComponent::manage_lifetime( )
    {
        if ( current_sprite_ptr_ == &spawn_sprite_ )
        {
            if ( spawn_sprite_.is_animation_completed( ) )
            {
                current_sprite_ptr_ = &default_sprite_;
            }
        }
        else if ( current_sprite_ptr_ == &default_sprite_ )
        {
            popped_ = default_sprite_.is_animation_completed( );
            speed_modifier_ = .5f;
            pathing_ = false;
        }
    }

}
