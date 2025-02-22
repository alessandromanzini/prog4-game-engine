#include "TextureComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"

using namespace engine;

TextureComponent::TextureComponent( GameObjectView&& owner, const std::string& filename )
	: BaseComponent( std::move( owner ) )
{
	texture_ptr_ = ResourceManager::get_instance( ).load_texture( filename );
}

void TextureComponent::render( ) const
{
	const auto pos = get_owner().get_transform( ).global( ).get_position( );
	Renderer::get_instance( ).render_texture( *texture_ptr_, pos.x, pos.y );
}
