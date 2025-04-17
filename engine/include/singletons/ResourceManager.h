#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <singletons/Singleton.h>

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <filesystem>
#include <map>
#include <string>


namespace engine
{
    class Texture2D;
    class Font;
    class ResourceManager final : public Singleton<ResourceManager>
    {
        friend class Singleton;
    public:
        ~ResourceManager( ) override = default;

        ResourceManager( const ResourceManager& )                = delete;
        ResourceManager( ResourceManager&& ) noexcept            = delete;
        ResourceManager& operator=( const ResourceManager& )     = delete;
        ResourceManager& operator=( ResourceManager&& ) noexcept = delete;

        void init( const std::filesystem::path& data );

        std::shared_ptr<Texture2D> load_texture( const std::string& file );
        std::shared_ptr<Font> load_font( const std::string& file, uint8_t size );

    private:
        std::filesystem::path data_path_;

        std::map<std::string, std::shared_ptr<Texture2D>> loaded_textures_;
        std::map<std::pair<std::string, uint8_t>, std::shared_ptr<Font>> loaded_fonts_;

        ResourceManager( ) = default;

        void unload_unused_resources( );

    };

    extern ResourceManager& RESOURCE_MANAGER;

}

#endif //!RESOURCEMANAGER_H
