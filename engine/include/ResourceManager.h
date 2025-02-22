#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <filesystem>
#include <string>
#include <memory>
#include <map>
#include "Singleton.h"

namespace engine
{
	class Texture2D;
	class Font;
	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		void init( const std::filesystem::path& data );
		std::shared_ptr<Texture2D> load_texture( const std::string& file );
		std::shared_ptr<Font> load_font( const std::string& file, uint8_t size );

		friend class Singleton<ResourceManager>;

	private:
		std::filesystem::path data_path_;

		std::map<std::string, std::shared_ptr<Texture2D>> loaded_textures_;
		std::map<std::pair<std::string, uint8_t>, std::shared_ptr<Font>> loaded_fonts_;

		ResourceManager( ) = default;

		void unload_unused_resources( );

	};
}

#endif // !RESOURCEMANAGER_H
