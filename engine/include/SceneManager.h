#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "Singleton.h"

#include <vector>
#include <string>
#include <memory>

namespace engine
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& create_scene( const std::string& name );

		void fixed_update( );
		void update( );
		void render( );

		Scene& get_active_scene( ) const;
		Scene& get_scene( const std::string& name ) const;
		Scene& get_scene( uint16_t id ) const;

		void cleanup( );

		friend class Singleton<SceneManager>;

	private:
		std::vector<std::shared_ptr<Scene>> scenes_;
		Scene* active_scene_ptr_{ nullptr };

		SceneManager( ) = default;

	};
}

#endif // !SCENE_MANAGER_H
