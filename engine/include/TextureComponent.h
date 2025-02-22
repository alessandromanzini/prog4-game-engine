#ifndef TEXTURECOMPONENT_H
#define TEXTURECOMPONENT_H

#include "BaseComponent.h"

#include <string>
#include <memory>

namespace engine
{
	class Texture2D;
	class TextureComponent final : public BaseComponent
	{
	public:
		explicit TextureComponent( GameObjectView&& owner, const std::string& filename );
		~TextureComponent( ) noexcept = default;

		TextureComponent( const TextureComponent& ) = delete;
		TextureComponent( TextureComponent&& ) noexcept = delete;
		TextureComponent& operator=( const TextureComponent& ) = delete;
		TextureComponent& operator=( TextureComponent&& ) noexcept = delete;

		void render( ) const override;

	private:
		std::shared_ptr<Texture2D> texture_ptr_;

	};
}

#endif // !TEXTURECOMPONENT_H
