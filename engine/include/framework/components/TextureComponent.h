#ifndef TEXTURECOMPONENT_H
#define TEXTURECOMPONENT_H

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include "Component.h"


namespace engine
{
    class Texture2D;
    class TextureComponent final : public Component
    {
    public:
        explicit TextureComponent( GameObjectView& owner, const std::string& filename );
        ~TextureComponent( ) noexcept override = default;

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
