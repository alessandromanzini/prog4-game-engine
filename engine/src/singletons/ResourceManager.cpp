#include "singletons/ResourceManager.h"

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include "Font.h"
#include "Texture2D.h"
#include "singletons/Renderer.h"

// +--------------------------------+
// | SDL HEADERS					|
// +--------------------------------+
#include <SDL_image.h>
#include <SDL_ttf.h>

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <stdexcept>


namespace fs = std::filesystem;

namespace engine
{
    ResourceManager& RESOURCE_MANAGER = ResourceManager::get_instance( );


    void ResourceManager::init( const std::filesystem::path& dataPath )
    {
        data_path_ = dataPath;

        if ( TTF_Init( ) != 0 )
        {
            throw std::runtime_error( std::string( "Failed to load support for fonts: " ) + SDL_GetError( ) );
        }
    }


    std::shared_ptr<Texture2D> ResourceManager::load_texture( const std::string& file )
    {
        const auto fullPath = data_path_ / file;
        const auto filename = fs::path( fullPath ).filename( ).string( );
        if ( not loaded_textures_.contains( filename ) )
        {
            loaded_textures_.insert( std::pair( filename, std::make_shared<Texture2D>( fullPath.string( ) ) ) );
        }
        return loaded_textures_.at( filename );
    }


    std::shared_ptr<Font> ResourceManager::load_font( const std::string& file, uint8_t size )
    {
        const auto fullPath = data_path_ / file;
        const auto filename = fs::path( fullPath ).filename( ).string( );
        const auto key      = std::pair<std::string, uint8_t>( filename, size );
        if ( not loaded_fonts_.contains( key ) )
        {
            loaded_fonts_.insert( std::pair( key, std::make_shared<Font>( fullPath.string( ), size ) ) );
        }
        return loaded_fonts_.at( key );
    }


    void ResourceManager::unload_unused_resources( )
    {
        for ( auto it = loaded_textures_.begin( ); it != loaded_textures_.end( ); )
        {
            if ( it->second.use_count( ) == 1 )
            {
                it = loaded_textures_.erase( it );
            }
            else
            {
                ++it;
            }
        }

        for ( auto it = loaded_fonts_.begin( ); it != loaded_fonts_.end( ); )
        {
            if ( it->second.use_count( ) == 1 )
            {
                it = loaded_fonts_.erase( it );
            }
            else
            {
                ++it;
            }
        }
    }

}
