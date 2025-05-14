#include <framework/component/TextureComponent.h>

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <framework/GameObject.h>
#include <singleton/Renderer.h>
#include <singleton/ResourceManager.h>


namespace engine
{
    TextureComponent::TextureComponent( owner_t& owner, const std::string& filename )
        : Component( owner )
        , texture_ptr_{ RESOURCE_MANAGER.load_texture( filename ) }
    {
    }


    void TextureComponent::render( ) const
    {
        const auto pos = get_owner( ).get_world_transform( ).get_position( );
        RENDERER.render_texture( *texture_ptr_, pos );
    }

}
