#ifndef SPRITE_TRANSITIONS_H
#define SPRITE_TRANSITIONS_H

#include <framework/behaviour/fsm/transitions.h>
#include <framework/resources/texture/Sprite2D.h>


namespace engine
{
    class GameObject;
}

namespace engine::fsm::states
{
    class SpriteState final : public State
    {
    public:
        explicit SpriteState( Sprite2D&& sprite );

        void on_enter( Blackboard& ) override;
        void on_exit( Blackboard& ) override;

        void tick( Blackboard& ) override;
        void render( Blackboard& ) const override;

    private:
        Sprite2D sprite_;
        GameObject* object_ptr_{ nullptr };
        RigidBodyComponent* rigid_body_ptr_{ nullptr };

    };

}


#endif //!SPRITE_TRANSITIONS_H
