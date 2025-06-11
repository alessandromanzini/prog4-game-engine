#ifndef BUBBLECAPTURECOMPONENT_H
#define BUBBLECAPTURECOMPONENT_H

#include <framework/component/Component.h>

#include <framework/resource/texture/Sprite2D.h>


namespace game
{
    struct BubbleCaptureResources final
    {
        engine::Sprite2D green_bubble_sprite;
        engine::Sprite2D blue_bubble_sprite;
        engine::Sprite2D yellow_bubble_sprite;
        engine::Sprite2D red_bubble_sprite;
        std::string fruit_texture_path{};
        int fruit_value{};
    };


    class BubbleCaptureComponent final : public engine::Component
    {
    public:
        explicit BubbleCaptureComponent( owner_t& owner, const BubbleCaptureResources& resources );

        void tick( ) override;
        void render( ) const override;

        void signal_captured( bool alt = false );
        void signal_yellow_stage( );
        void signal_red_stage( );
        void signal_released( );

        [[nodiscard]] bool is_captured( ) const;
        [[nodiscard]] const std::string& get_fruit_texture_path( ) const;
        [[nodiscard]] int get_fruit_value( ) const;

    private:
        BubbleCaptureResources resources_;
        bool captured_{ false };
        engine::Sprite2D* current_sprite_ptr_{ nullptr };

    };

}


#endif //!BUBBLECAPTURECOMPONENT_H
