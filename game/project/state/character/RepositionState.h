#ifndef REPOSITIONSTATE_H
#define REPOSITIONSTATE_H

#include <state/__base/CharacterState.h>


namespace game
{
    class RepositionState final : public CharacterState
    {
    public:
        explicit RepositionState( engine::Sprite2D* sprite, engine::AudioComponent* audio, glm::vec2 target );
        void on_enter( engine::Blackboard& ) override;
        void on_exit( engine::Blackboard& ) override;
        void tick( engine::Blackboard& ) override;

    private:
        static constexpr float REPOSITION_SPEED{ 100.f };
        const glm::vec2 target_{};
        glm::vec2 direction_{};

    };

}


#endif //!REPOSITIONSTATE_H
