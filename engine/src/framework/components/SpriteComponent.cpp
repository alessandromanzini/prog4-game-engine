#include <framework/components/SpriteComponent.h>

#include <framework/GameObject.h>
#include <singletons/GameTime.h>
#include <singletons/Renderer.h>
#include <singletons/ResourceManager.h>


namespace engine
{
    SpriteComponent::SpriteComponent( owner_t& owner, const std::string& filename, const uint8_t rows, const uint8_t cols, const float frameDelay )
        : Component( owner )
        , rows_{ rows }
        , cols_{ cols }
        , total_frames_{ static_cast<uint16_t>( rows * cols ) }
        , frame_delay_{ frameDelay }
        , texture_ptr_{ RESOURCE_MANAGER.load_texture( filename ) }
    {
        assert( frame_delay_ > 0.f && "Frame delay must be greater than 0" );
        assert( total_frames_ > 0 && "Total frames must be greater than 0" );

        frame_width_ = texture_ptr_->get_size(  ).x / cols_;
        frame_height_ = texture_ptr_->get_size(  ).y / rows_;
    }


    void SpriteComponent::render( ) const
    {
        const auto pos = get_owner( ).get_world_transform( ).get_position( );
        const glm::vec4 srcRect{
            static_cast<float>( current_frame_ % cols_ ) * frame_width_,
            static_cast<float>( current_frame_ / cols_ ) * frame_height_,
            frame_width_,
            frame_height_
        };
        RENDERER.render_partial_texture( *texture_ptr_, pos, srcRect );
    }


    void SpriteComponent::update( )
    {
        accumulated_time_ += GAME_TIME.get_delta_time(  );
        if ( accumulated_time_ >= frame_delay_ )
        {
            accumulated_time_ -= frame_delay_;
            current_frame_ = ( current_frame_ + 1 ) % total_frames_;
        }
    }

}
