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
        engine::Sprite2D reposition_sprite;

        std::string attack_audio_path{};
        std::string jump_audio_path{};
        std::string bounce_audio_path{};
    };


    class CharacterComponent final : public engine::Component
    {
    public:
        explicit CharacterComponent( owner_t& owner, const CharacterResources& resources,
            std::unique_ptr<AttackCommand>&& attackCommand, std::unique_ptr<JumpCommand>&& jumpCommand,
            std::unique_ptr<MoveCommand>&& moveCommand, bool attackOnEnter );

        void tick( ) override;
        void render( ) const override;

        void set_start_position( glm::vec2 position );
        [[nodiscard]] glm::vec2 get_relative_movement( ) const;

        void move( glm::vec2 movement ) const;
        void jump( );
        void attack( );

        void interrupt( );
        void reposition( );
        void lock( );

        void set_physics_simulation( bool simulate ) const;

        [[nodiscard]] bool is_repositioning( ) const;
        [[nodiscard]] bool is_locked( ) const;
        [[nodiscard]] bool is_falling( ) const;
        [[nodiscard]] std::pair<bool, float> is_iframing( ) const;

    private:
        static constexpr float IFRAMES_SWAP_{ .2f };

        engine::Blackboard blackboard_{};
        engine::FiniteStateMachine state_machine_{ blackboard_ };

        CharacterResources resources_;
        std::unique_ptr<AttackCommand> attack_command_ptr_{ nullptr };
        std::unique_ptr<JumpCommand> jump_command_ptr_{ nullptr };
        std::unique_ptr<MoveCommand> move_command_ptr_{ nullptr };
        const bool attack_on_enter_{ false };

        glm::vec2 size_{};
        std::array<glm::vec2, 4> prev_positions_{};
        uint8_t current_position_index_{ 0 };
        glm::vec2 running_sum_{ 0.f };

        float iframe_swap_accumulated_{};
        bool is_iframe_swap_{};

        void fill_running_sum( );
        void initialize_resources( );
        void create_transitions( );

        void vertical_position_wrap( ) const;
        void average_positions( );
        void manage_iframes( );

    };
}


#endif //!BUBSTATECOMPONENT_H
