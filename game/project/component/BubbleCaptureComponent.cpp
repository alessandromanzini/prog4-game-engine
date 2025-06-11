#include "BubbleCaptureComponent.h"

#include <framework/GameObject.h>
#include <singleton/Renderer.h>


namespace game
{
    BubbleCaptureComponent::BubbleCaptureComponent( owner_t& owner, const BubbleCaptureResources& resources )
        : Component{ owner }
        , resources_{ resources } { }


    void BubbleCaptureComponent::tick( )
    {
        if ( captured_ )
        {
            current_sprite_ptr_->tick(  );
        }
    }


    void BubbleCaptureComponent::render( ) const
    {
        if ( captured_ )
        {
            engine::RENDERER.set_z_index( 3 );
            current_sprite_ptr_->render( get_owner( ).get_world_transform( ).get_position( ) );
            engine::RENDERER.set_z_index( 0 );
        }
    }


    void BubbleCaptureComponent::signal_captured( const bool alt )
    {
        captured_ = true;
        current_sprite_ptr_ = alt ? &resources_.blue_bubble_sprite : &resources_.green_bubble_sprite;
    }


    void BubbleCaptureComponent::signal_yellow_stage( )
    {
        current_sprite_ptr_ = &resources_.yellow_bubble_sprite;
    }


    void BubbleCaptureComponent::signal_red_stage( )
    {
        current_sprite_ptr_ = &resources_.red_bubble_sprite;
    }


    void BubbleCaptureComponent::signal_released( )
    {
        captured_ = false;
        current_sprite_ptr_ = nullptr;
    }


    bool BubbleCaptureComponent::is_captured( ) const
    {
        return captured_;
    }


    const std::string& BubbleCaptureComponent::get_fruit_texture_path( ) const
    {
        return resources_.fruit_texture_path;
    }


    int BubbleCaptureComponent::get_fruit_value( ) const
    {
        return resources_.fruit_value;
    }

}
