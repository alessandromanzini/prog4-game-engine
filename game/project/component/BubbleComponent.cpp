#include "BubbleComponent.h"

#include "BubbleCaptureComponent.h"

#include <framework/GameObject.h>
#include <framework/component/physics/BoxColliderComponent.h>
#include <singleton/GameTime.h>
#include <singleton/Renderer.h>

#include "CharacterComponent.h"


namespace game
{
    BubbleComponent::BubbleComponent( owner_t& owner, const glm::vec2 initialDirection, const float bubbleSpeed )
        : Component{ owner }
        , bubble_speed_{ bubbleSpeed }
        , direction_{ initialDirection }
        , spawn_sprite_{ "characters/bubble/bubble_spawn_1x6.png", 1u, 6u, 0.15f, 2.f, { -14.f, -16.f }, false }
        , default_sprite_{ "characters/bubble/bubble_lifetime_1x3.png", 1u, 3u, 2.f, 2.f, { -14.f, -16.f }, false } { }


    void BubbleComponent::tick( )
    {
        const float speed{ bubble_speed_ * speed_modifier_ * engine::GAME_TIME.get_delta_time( ) };
        const glm::vec2 direction{ pathing_ ? direction_ : glm::vec2{ 0.f, -1.f } };
        get_owner( ).get_transform_operator( ).translate( direction * speed );

        manage_lifetime( );

        current_sprite_ptr_->tick( );
    }


    void BubbleComponent::render( ) const
    {
        if ( not popped_ )
        {
            engine::RENDERER.set_z_index( 5 );
            current_sprite_ptr_->render( get_owner( ).get_world_transform( ).get_position( ) );
            engine::RENDERER.set_z_index( 0 );
        }
    }


    void BubbleComponent::bounce( const glm::vec2 normal, const float depth )
    {
        if ( dot( normal, glm::vec2{ 0.f, 1.f } ) == 1.f )
        {
            get_owner( ).get_transform_operator( ).translate( normal * ( depth + .1f ) );
            pathing_ = true;
        }
        else
        {
            direction_ = normal;
        }
    }


    bool BubbleComponent::has_captured_target( ) const
    {
        return captured_target_ptr_;
    }


    BubbleCaptureComponent* BubbleComponent::get_captured_target( ) const
    {
        return bubble_capture_component_ptr_;
    }


    void BubbleComponent::capture( engine::GameObject& target )
    {
        if ( has_captured_target( ) )
        {
            return;
        }
        captured_target_ptr_ = &target;
        captured_target_ptr_->set_parent( &get_owner( ) );
        captured_target_ptr_->set_local_transform( engine::Transform::from_translation( { -14.f, -16.f } ) );

        captured_character_component_ptr_ = &captured_target_ptr_->get_component<CharacterComponent>( );
        bubble_capture_component_ptr_ = &captured_target_ptr_->get_component<BubbleCaptureComponent>( );
        assert( captured_character_component_ptr_ && bubble_capture_component_ptr_ &&
                "BubbleComponent::capture: Target must have CharacterComponent and BubbleCaptureComponent!" );

        captured_character_component_ptr_->set_physics_simulation( false );
        captured_character_component_ptr_->interrupt( );
        captured_character_component_ptr_->lock( );

        current_sprite_ptr_ = &default_sprite_;
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
            popped_         = default_sprite_.is_animation_completed( );
            speed_modifier_ = .5f;
            pathing_        = false;
        }

        if ( popped_ )
        {
            if ( has_captured_target( ) )
            {
                captured_target_ptr_->set_parent( nullptr );
                captured_character_component_ptr_->set_physics_simulation( true );
                captured_character_component_ptr_->interrupt( );
            }
            get_owner( ).mark_for_deletion( );
        }
    }

}
