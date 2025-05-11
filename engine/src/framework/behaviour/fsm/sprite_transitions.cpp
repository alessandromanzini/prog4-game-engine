#include <framework/behaviour/fsm/sprite_transitions.h>

#include <framework/GameObject.h>
#include <framework/components/RigidBodyComponent.h>
#include <framework/resources/data/blackboard_ids.h>
#include <framework/resources/texture/Sprite2D.h>


namespace engine::fsm::states
{
    SpriteState::SpriteState( Sprite2D&& sprite )
        : sprite_( std::move( sprite ) ) { }


    void SpriteState::on_enter( Blackboard& blackboard )
    {
        blackboard.retrieve( ids::OBJECT_UID, object_ptr_ );
        blackboard.retrieve( ids::RIGID_BODY_UID, rigid_body_ptr_ );
        assert( object_ptr_ && "SpriteState::on_enter: Missing object pointer" );

        sprite_.reset(  );
    }


    void SpriteState::on_exit( Blackboard& )
    {
        object_ptr_ = nullptr;
        rigid_body_ptr_ = nullptr;
    }


    void SpriteState::tick( Blackboard& )
    {
        sprite_.tick(  );

        if ( rigid_body_ptr_ )
        {
            sprite_.flip( rigid_body_ptr_->get_face_direction( ) == FaceDirection::RIGHT, false );
        }
    }


    void SpriteState::render( Blackboard& ) const
    {
        sprite_.render( object_ptr_->get_world_transform( ).get_position( ) );
    }

}
