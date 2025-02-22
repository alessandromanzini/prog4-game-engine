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

		void cleanup( );

		friend class Singleton<SceneManager>;

	private:
		std::vector<std::shared_ptr<Scene>> scenes_;

		SceneManager( ) = default;

	};
}

#endif // !SCENE_MANAGER_H
