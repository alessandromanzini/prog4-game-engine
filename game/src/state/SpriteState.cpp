#include <state/SpriteState.h>

#include <framework/component/physics/RigidBodyComponent.h>
#include <framework/resource/data/blackboard_ids.h>


namespace game
{
    SpriteState::SpriteState( engine::Sprite2D* sprite, const engine::UID spriteType )
        : sprite_ptr_{ sprite }
        , sprite_type_{ spriteType } { }


    void SpriteState::on_enter( engine::Blackboard& blackboard )
    {
        sprite_ptr_->reset( );
        subject_.broadcast( sprite_type_ );

        blackboard.retrieve( engine::ids::RIGID_BODY_UID, rigid_body_ptr_ );
        assert( rigid_body_ptr_ && "SpriteState::on_enter: Missing object pointer" );
    }


    void SpriteState::on_exit( engine::Blackboard& )
    {
        subject_.broadcast( engine::NULL_UID );
        rigid_body_ptr_ = nullptr;
    }


    void SpriteState::tick( engine::Blackboard& )
    {
        // TODO: reimplement this
        // sprite_ptr_->flip( rigid_body_ptr_->get_face_direction( ) == engine::FaceDirection::RIGHT, false );
    }


    void SpriteState::add_observer( engine::Observer& observer )
    {
        subject_.add_observer( observer );
    }


    void SpriteState::remove_observer( const engine::Observer& observer )
    {
        subject_.remove_observer( observer );
    }

}
