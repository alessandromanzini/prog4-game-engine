#include "CharacterState.h"

#include <framework/component/AudioComponent.h>


namespace game
{
    CharacterState::CharacterState( engine::Sprite2D* sprite, engine::AudioComponent* audio, const bool canMove, const bool canJump, const bool canAttack )
        : can_move_{ canMove }
        , can_jump_{ canJump }
        , can_attack_{ canAttack }
        , audio_ptr_{ audio }
    {
        set_current_sprite( sprite );
    }


    bool CharacterState::can_move( ) const
    {
        return can_move_;
    }


    bool CharacterState::can_jump( ) const
    {
        return can_jump_;
    }


    bool CharacterState::can_attack( ) const
    {
        return can_attack_;
    }


    void CharacterState::on_enter( engine::Blackboard& blackboard )
    {
        sprite_ptr_->reset( );
        blackboard.edit( engine::UID( "current_sprite" ), sprite_ptr_ );

        if ( audio_ptr_ )
        {
            audio_ptr_->play( );
        }
    }


    void CharacterState::tick( engine::Blackboard& )
    {
        if ( sprite_ptr_ )
        {
            sprite_ptr_->tick( );
        }
    }


    void CharacterState::set_current_sprite( engine::Sprite2D* sprite )
    {
        sprite_ptr_ = sprite;
    }

}
