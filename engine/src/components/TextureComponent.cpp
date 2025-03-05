#include "components/TextureComponent.h"

// +--------------------------------+
// | Project Headers				|
// +--------------------------------+
#include "singletons/ResourceManager.h"
#include "singletons/Renderer.h"

using namespace engine;

TextureComponent::TextureComponent( GameObjectView& owner, const std::string& filename )
	: BaseComponent( owner )
{
	texture_ptr_ = ResourceManager::get_instance( ).load_texture( filename );
}

void TextureComponent::render( ) const
{
	const auto pos = get_owner().get_world_transform( ).get_position( );
	Renderer::get_instance( ).render_texture( *texture_ptr_, pos.x, pos.y );
}
