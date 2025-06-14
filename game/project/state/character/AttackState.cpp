#include "AttackState.h"

#include <command/AttackCommand.h>


namespace game
{
    AttackState::AttackState( engine::Sprite2D* sprite, engine::AudioComponent* audio, AttackCommand* attack,
                              const bool attackOnEnter )
        : CharacterState{ sprite, audio, true, false, false }
        , attack_on_enter_{ attackOnEnter }
        , attack_command_ptr_{ attack }
    {
        assert( attack_command_ptr_ && "AttackState requires an AttackCommand to be set!" );
    }


    void AttackState::on_enter( engine::Blackboard& blackboard )
    {
        CharacterState::on_enter( blackboard );
        bool interrupt{ false };
        blackboard.retrieve( engine::UID( "interrupt" ), interrupt );
        if ( not interrupt && attack_on_enter_ )
        {
            attack_command_ptr_->execute( );
        }
    }


    void AttackState::on_exit( engine::Blackboard& blackboard )
    {
        CharacterState::on_exit( blackboard );
        bool interrupt{ false };
        blackboard.retrieve( engine::UID( "interrupt" ), interrupt );
        if (  not interrupt && not attack_on_enter_ )
        {
            attack_command_ptr_->execute( );
        }
    }

}
