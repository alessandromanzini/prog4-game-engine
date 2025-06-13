#ifndef SPRITESTATE_H
#define SPRITESTATE_H

#include <framework/behaviour/fsm/transitions.h>
#include <framework/resource/texture/Sprite2D.h>


namespace engine
{
    class AudioComponent;
}

namespace game
{
    class CharacterState : public engine::fsm::State
    {
    public:
        explicit CharacterState( engine::Sprite2D* sprite, engine::AudioComponent* audio, bool canMove, bool canJump, bool canAttack );

        void on_enter( engine::Blackboard& ) override;
        void tick( engine::Blackboard& ) override;

        [[nodiscard]] bool can_move( ) const;
        [[nodiscard]] bool can_jump( ) const;
        [[nodiscard]] bool can_attack( ) const;

    protected:
        void set_current_sprite( engine::Sprite2D* sprite );

    private:
        const bool can_move_{};
        const bool can_jump_{};
        const bool can_attack_{};

        engine::Sprite2D* sprite_ptr_{ nullptr };
        engine::AudioComponent* audio_ptr_{ nullptr };

    };

}


#endif //!SPRITESTATE_H
