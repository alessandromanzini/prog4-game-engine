#include "JumpState.h"

#include <command/JumpCommand.h>


namespace game
{
    JumpState::JumpState( engine::AudioComponent* audio, JumpCommand* jump )
        : CharacterState{ nullptr, audio, true, false, false }
        , jump_command_ptr_{ jump }
    {
        assert( jump_command_ptr_ && "JumpState requires a JumpCommand to be set!" );
    }


    void JumpState::on_enter( engine::Blackboard& blackboard )
    {
        CharacterState::on_enter( blackboard );
        jump_command_ptr_->execute( );
    }


}
