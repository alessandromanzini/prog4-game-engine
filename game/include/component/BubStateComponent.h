#ifndef BUBSTATECOMPONENT_H
#define BUBSTATECOMPONENT_H

#include <framework/behaviour/fsm/FiniteStateMachine.h>
#include <framework/component/Component.h>
#include <framework/event/Observer.h>
#include <framework/resource/texture/Sprite2D.h>


namespace game
{
    namespace state
    {
        enum class CharacterState
        {
            IDLE,
            WALK,
            JUMP,
            FALL
        };

    }

    class BubStateComponent final : public engine::Component, public engine::Observer
    {
    public:
        explicit BubStateComponent( owner_t& owner );

        void tick( ) override;
        void render( ) const override;

        void notify( engine::UID event, engine::Subject* subject, engine::event::broadcast_value_variant_t value ) override;

    private:
        engine::Blackboard blackboard_{};
        engine::FiniteStateMachine state_machine_;

        std::map<engine::UID, engine::Sprite2D> sprites_{};
        engine::Sprite2D* sprite_ptr_{ nullptr };

        void add_sprite_state( engine::UID uid );

    };
}


#endif //!BUBSTATECOMPONENT_H
