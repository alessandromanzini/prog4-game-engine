#ifndef BUBBLECAPTURECOMPONENT_H
#define BUBBLECAPTURECOMPONENT_H

#include <framework/component/Component.h>

#include <framework/resource/texture/Sprite2D.h>


namespace game
{
    struct BubbleCaptureResources final
    {
        std::string fruit_texture_path{};
        int fruit_value{};
    };


    class BubbleCaptureComponent final : public engine::Component
    {
    public:
        explicit BubbleCaptureComponent( owner_t& owner, const BubbleCaptureResources& resources );
        [[nodiscard]] const std::string& get_fruit_texture_path( ) const;
        [[nodiscard]] int get_fruit_value( ) const;

    private:
        BubbleCaptureResources resources_;

    };

}


#endif //!BUBBLECAPTURECOMPONENT_H
