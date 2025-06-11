#ifndef TEXTCOMPONENT_H
#define TEXTCOMPONENT_H

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <framework/component/Component.h>

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <memory>
#include <string>


namespace engine
{
    class Font;
    class Texture2D;
    class TextComponent final : public Component
    {
    public:
        explicit TextComponent( owner_t& owner, const std::string& text, std::shared_ptr<Font> font );

        void tick( ) override;
        void render( ) const override;

        void set_text( const std::string& text );

    private:
        bool needs_update_{};
        std::string text_;

        std::shared_ptr<Font> font_ptr_{ nullptr };
        std::shared_ptr<Texture2D> text_texture_ptr_{ nullptr };
    };
}

#endif //!TEXTCOMPONENT_H
