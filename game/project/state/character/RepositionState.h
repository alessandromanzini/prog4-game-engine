#ifndef REPOSITIONSTATE_H
#define REPOSITIONSTATE_H

#include <state/__base/CharacterState.h>


namespace game
{
    class RepositionState final : public CharacterState
    {
    public:
        explicit RepositionState( engine::Sprite2D* sprite, engine::AudioComponent* audio );
        void on_enter( engine::Blackboard& ) override;
        void on_exit( engine::Blackboard& ) override;
        void tick( engine::Blackboard& ) override;

    private:
        static constexpr float REPOSITION_TIME_{ 4.f };
        static constexpr float REPOSITION_IFRAMES_{ 1.25f };
        glm::vec2 direction_{};
        float accumulator_{ 0.f };

    };

}


#endif //!REPOSITIONSTATE_H
