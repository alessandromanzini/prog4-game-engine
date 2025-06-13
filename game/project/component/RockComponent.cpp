#include "RockComponent.h"

#include <framework/GameObject.h>
#include <framework/component/physics/BoxColliderComponent.h>
#include <singleton/GameTime.h>


namespace game
{
    RockComponent::RockComponent( owner_t& owner, const glm::vec2 initialDirection, const float speed )
        : Component( owner )
        , rock_speed_{ speed }
        , direction_{ initialDirection }
        , default_sprite_{ "characters/maita/maita_rock_loop_1x4.png", 1u, 4u, 0.15f, 2.f, { -15.f, -16.f } }
        , destory_sprite_{ "characters/maita/maita_rock_destroy_1x3.png", 1u, 3u, 0.1f, 2.f, { -15.f, -16.f }, false } { }


    void RockComponent::tick( )
    {
        manage_lifetime( );
        current_sprite_ptr_->tick( );
        if (current_sprite_ptr_ == &default_sprite_)
        {
            get_owner( ).get_transform_operator( ).translate( direction_ * rock_speed_ * engine::GAME_TIME.get_delta_time( ) );
        }
    }


    void RockComponent::render( ) const
    {
        current_sprite_ptr_->render( get_owner( ).get_world_transform( ).get_position( ) );
    }


    void RockComponent::destroy( )
    {
        current_sprite_ptr_ = &destory_sprite_;
        get_owner( ).get_component<engine::BoxColliderComponent>( ).value( ).mark_for_deletion( );
    }


    void RockComponent::manage_lifetime( ) const
    {
        if ( current_sprite_ptr_ == &destory_sprite_ && destory_sprite_.is_animation_completed( ) )
        {
            get_owner( ).mark_for_deletion( );
        }
    }

}
