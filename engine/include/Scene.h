#ifndef SCENE_H
#define SCENE_H

#include <unordered_set>

#include "SceneManager.h"

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
		bool is_cleanup_needed( ) const noexcept;

		friend Scene& SceneManager::create_scene( const std::string& name );

	private:
		std::string name_;
		std::vector<std::shared_ptr<GameObject>> objects_{};
		std::unordered_set<size_t> indices_to_destroy_{};	// We choose a set because we don't want duplicates and we retain O(1) complexity for insertion, deletion, and search.
						
		static unsigned int m_idCounter;

		explicit Scene( const std::string& name );

		void mark_index_for_deletion( size_t index );

	};

}

#endif // !SCENE_H
