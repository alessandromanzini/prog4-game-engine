#ifndef ATTACKSTATE_H
#define ATTACKSTATE_H

#include <state/__base/CharacterState.h>


namespace game
{
    class AttackCommand;
}

namespace game
{
    class AttackState final : public CharacterState
    {
    public:
        explicit AttackState( engine::Sprite2D* sprite, engine::AudioComponent* audio, AttackCommand* attack, bool attackOnEnter );
        void on_enter( engine::Blackboard& ) override;
        void on_exit( engine::Blackboard& ) override;

    private:
        const bool attack_on_enter_{ false };
        AttackCommand* attack_command_ptr_{ nullptr };

    };

}


#endif //!ATTACKSTATE_H
