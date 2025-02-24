#ifndef SCENE_H
#define SCENE_H

#include "SceneManager.h"
#include "Deleter.h"

namespace engine
{
	class GameObject;
	class Scene final
	{
	public:
		Scene( ) = default;
		~Scene( ) noexcept = default;
		
		Scene( const Scene& )					= delete;
		Scene( Scene&& ) noexcept				= delete;
		Scene& operator=( const Scene& ) 		= delete;
		Scene& operator=( Scene&& ) noexcept 	= delete;

		void add( std::shared_ptr<GameObject> object );
		void remove( std::shared_ptr<GameObject> object );
		void remove_all( );

		void fixed_update( );
		void update( );
		void render( ) const;

		void cleanup( );

		friend Scene& SceneManager::create_scene( const std::string& name );

	private:
		std::string name_;
		std::vector<std::shared_ptr<GameObject>> objects_{};

		Deleter<GameObject> deleter_{};
						
		static unsigned int m_idCounter;

		explicit Scene( const std::string& name );

	};

}

#endif // !SCENE_H
