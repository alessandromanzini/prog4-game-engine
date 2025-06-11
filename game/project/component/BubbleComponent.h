#ifndef BUBBLECOMPONENT_H
#define BUBBLECOMPONENT_H

#include <framework/component/Component.h>
#include <framework/resource/texture/Sprite2D.h>

#include <vec2.hpp>


namespace game
{
    class BubbleCaptureComponent;
    class BubbleComponent final : public engine::Component
    {
    public:
        explicit BubbleComponent( owner_t& owner, glm::vec2 initialDirection, float bubbleSpeed );
        void tick( ) override;
        void render( ) const override;

        void bounce( glm::vec2 normal, float depth );

        [[nodiscard]] bool has_captured_target( ) const;
        [[nodiscard]] BubbleCaptureComponent* get_captured_target( ) const;
        void capture( engine::GameObject& target );

    private:
        const float bubble_speed_{};
        float speed_modifier_{ 1.f };
        glm::vec2 direction_{};

        bool pathing_{ true };

        engine::Sprite2D spawn_sprite_;
        engine::Sprite2D default_sprite_;
        engine::Sprite2D* current_sprite_ptr_{ &spawn_sprite_ };

        engine::GameObject* captured_target_ptr_{ nullptr };
        BubbleCaptureComponent* bubble_capture_component_ptr_{ nullptr };

        bool popped_{ false  };

        void manage_lifetime( );

    };

}


#endif //!BUBBLECOMPONENT_H
