#ifndef BUBSTATECOMPONENT_H
#define BUBSTATECOMPONENT_H

#include <framework/component/Component.h>

#include <state/__base/CharacterState.h>

#include <command/AttackCommand.h>
#include <command/JumpCommand.h>
#include <command/MoveCommand.h>

#include <framework/behaviour/fsm/FiniteStateMachine.h>

#include <array>


namespace engine
{
    class AudioComponent;
}

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

    struct CharacterResources final
    {
        engine::Sprite2D idle_sprite;
        engine::Sprite2D walk_sprite;
        engine::Sprite2D rise_sprite;
        engine::Sprite2D fall_sprite;
        engine::Sprite2D attack_sprite;

        std::string attack_audio_path{};
        std::string jump_audio_path{};
        std::string bounce_audio_path{};
    };


    class CharacterComponent final : public engine::Component
    {
    public:
        explicit CharacterComponent( owner_t& owner, const CharacterResources& resources,
            std::unique_ptr<AttackCommand>&& attackCommand, std::unique_ptr<JumpCommand>&& jumpCommand,
            std::unique_ptr<MoveCommand>&& moveCommand );

        void tick( ) override;
        void render( ) const override;

        void move( glm::vec2 movement ) const;
        void jump( ) const;
        void attack( );

        void set_physics_simulation( bool simulate ) const;

    private:
        engine::Blackboard blackboard_{};
        engine::FiniteStateMachine state_machine_{ blackboard_ };

        std::array<glm::vec2, 4> prev_positions_{};
        uint8_t current_position_index_{ 0 };
        glm::vec2 running_sum_{ 0.f };

        CharacterResources resources_;
        std::unique_ptr<AttackCommand> attack_command_ptr_{ nullptr };
        std::unique_ptr<JumpCommand> jump_command_ptr_{ nullptr };
        std::unique_ptr<MoveCommand> move_command_ptr_{ nullptr };

        engine::AudioComponent* attack_audio_ptr_{ nullptr };
        engine::AudioComponent* jump_audio_ptr_{ nullptr };

        void initialize_resources( );
        void create_transitions( );

        void average_positions( );

    };
}


#endif //!BUBSTATECOMPONENT_H
