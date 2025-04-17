#ifndef TEXTURECOMPONENT_H
#define TEXTURECOMPONENT_H

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <framework/components/Component.h>

#include <string>


namespace engine
{
    class Texture2D;
    class TextureComponent final : public Component
    {
    public:
        explicit TextureComponent( owner_t& owner, const std::string& filename );
        ~TextureComponent( ) override = default;

        TextureComponent( const TextureComponent& )                = delete;
        TextureComponent( TextureComponent&& ) noexcept            = delete;
        TextureComponent& operator=( const TextureComponent& )     = delete;
        TextureComponent& operator=( TextureComponent&& ) noexcept = delete;

        void render( ) const override;

    private:
        std::shared_ptr<Texture2D> texture_ptr_;

    };
}

#endif //!TEXTURECOMPONENT_H
