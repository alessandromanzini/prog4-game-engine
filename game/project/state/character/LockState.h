#ifndef LOCKSTATE_H
#define LOCKSTATE_H

#include <state/__base/CharacterState.h>


namespace game
{
    class LockState final : public CharacterState
    {
    public:
        explicit LockState( engine::Sprite2D* sprite, engine::AudioComponent* audio );
        void on_enter( engine::Blackboard& ) override;
        void on_exit( engine::Blackboard& ) override;

    };

}


#endif //!LOCKSTATE_H
