#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <framework/components/Component.h>
#include <framework/resources/Texture2D.h>

#include <memory>


namespace engine
{
    class SpriteComponent final : public Component
    {
    public:
        explicit SpriteComponent( owner_t& owner, const std::string& filename, uint8_t rows, uint8_t cols, float frameDelay );

        void render( ) const override;
        void update( ) override;

    private:
        const uint8_t rows_{};
        const uint8_t cols_{};
        const uint16_t total_frames_{};
        const float frame_delay_{};

        std::shared_ptr<Texture2D> texture_ptr_;

        float frame_width_{};
        float frame_height_{};

        float accumulated_time_{ 0.0f };
        int current_frame_{ 0 };

    };

}


#endif //!SPRITECOMPONENT_H
