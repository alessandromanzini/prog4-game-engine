#ifndef BUBSTATECOMPONENT_H
#define BUBSTATECOMPONENT_H

#include <framework/component/Component.h>

#include <state/__base/CharacterState.h>

#include <command/AttackCommand.h>
#include <command/JumpCommand.h>
#include <command/MoveCommand.h>

#include <framework/behaviour/fsm/FiniteStateMachine.h>


namespace game
{
    namespace state
    {
        enum class Action
        {
            GROUNDED,
            AIRBORNE,

            IDLE,
            WALK,
            JUMP,
            RISE,
            FALL
        };

    }

    class CharacterComponent final : public engine::Component
    {
    public:
        explicit CharacterComponent( owner_t& owner, engine::GameObject& projectileSocket );

        void tick( ) override;
        void render( ) const override;

        void move( float movement );
        void jump( );
        void attack( );

    private:
        static constexpr float PROJECTILE_SPEED_{ 100.f };
        static constexpr float PROJECTILE_OFFSET_{ 20.f };
        static constexpr float JUMP_FORCE_{ 300.f };
        static constexpr float MOVEMENT_SPEED_{ 80.f };

        engine::Blackboard blackboard_{};
        engine::FiniteStateMachine state_machine_;

        std::array<glm::vec2, 4> prev_positions_{};
        uint8_t current_position_index_{ 0 };
        glm::vec2 running_sum_{ 0.f };

        AttackCommand attack_command_;
        JumpCommand jump_command_;
        MoveCommand move_command_;

        engine::Sprite2D idle_sprite_;
        engine::Sprite2D walk_sprite_;
        engine::Sprite2D rise_sprite_;
        engine::Sprite2D fall_sprite_;
        engine::Sprite2D attack_sprite_;

        void average_positions( );

    };
}


#endif //!BUBSTATECOMPONENT_H
