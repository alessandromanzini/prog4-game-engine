#ifndef JUMPSTATE_H
#define JUMPSTATE_H

#include <state/__base/CharacterState.h>


namespace game
{
    class JumpCommand;
}

namespace game
{
    class JumpState final : public CharacterState
    {
    public:
        explicit JumpState( engine::AudioComponent* audio, JumpCommand* jump );
        void on_enter( engine::Blackboard& ) override;

    private:
        JumpCommand* jump_command_ptr_{ nullptr };

    };

}


#endif //!JUMPSTATE_H
