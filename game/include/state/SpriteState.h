#ifndef SPRITESTATE_H
#define SPRITESTATE_H

#include <framework/behaviour/fsm/transitions.h>
#include <framework/event/Subject.h>
#include <framework/resource/texture/Sprite2D.h>


namespace game
{
    class SpriteState final : public engine::fsm::State
    {
    public:
        explicit SpriteState( engine::Sprite2D* sprite, engine::UID spriteType );

        void on_enter( engine::Blackboard& ) override;
        void on_exit( engine::Blackboard& ) override;
        void tick( engine::Blackboard& ) override;

        void add_observer( engine::Observer& observer );
        void remove_observer( const engine::Observer& observer );

    private:
        engine::Sprite2D* sprite_ptr_{ nullptr };
        engine::UID sprite_type_{ engine::NULL_UID };

        engine::RigidBodyComponent* rigid_body_ptr_{ nullptr };

        engine::Subject subject_{};


    };

}


#endif //!SPRITESTATE_H
