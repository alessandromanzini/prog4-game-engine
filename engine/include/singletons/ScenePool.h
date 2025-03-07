#ifndef SCENEPOOL_H
#define SCENEPOOL_H

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include "Singleton.h"
#include "Scene.h"

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <vector>
#include <string>
#include <memory>

namespace engine
{
	class ScenePool final : public Singleton<ScenePool>
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

		friend class Singleton<ScenePool>;

	private:
		std::vector<std::unique_ptr<Scene>> scenes_;
		Scene* active_scene_ptr_{ nullptr };

		ScenePool( ) = default;

	};

	extern ScenePool& SCENE_POOL;
}

#endif // !SCENEPOOL_H
