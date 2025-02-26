#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

// +--------------------------------+
// | Standard Headers				|
// +--------------------------------+
#include <string>
#include <memory>
#include <unordered_map>
#include <stdexcept>

// +--------------------------------+
// | Project Headers				|
// +--------------------------------+
#include "Deleter.h"
#include "GameObject.h"
#include "Transform.h"
#include "type_utility.hpp"

namespace engine
{
	class BaseComponent;
	class GameObjectView;
	class GameObject final
	{
	public:
		GameObject( );
		~GameObject( ) noexcept;

		GameObject( const GameObject& other ) = delete;
		GameObject( GameObject&& other ) noexcept = delete;
		GameObject& operator=( const GameObject& other ) = delete;
		GameObject& operator=( GameObject&& other ) noexcept = delete;

		void fixed_update( );
		void update( );
		void render( ) const;

		void cleanup( );

		void set_parent( GameObject* pParent, bool keepWorldPosition = true );

		[[nodiscard]] const Transform& get_world_transform( );
		[[nodiscard]] const Transform& get_local_transform( ) const;
		void set_world_transform( const Transform& transform );
		void set_local_transform( const Transform& transform );
		void set_local_transform( Transform&& transform );

		template <typename component_t, typename... args_t>
		void add_component( args_t&&... args )
		{
			// Initializing component with GameObjectView and arguments' perfect forwarding.
			// We insert the element in the multimap with its compile-time type hash as the key.
			auto component = std::make_unique<component_t>( *view_ptr_, std::forward<args_t>( args )... );
			components_.insert( std::make_pair( type_utility::type_hash<component_t>( ), std::move( component ) ) );
		}

		template <typename component_t>
		component_t& get_component( ) const
		{
			// We find the component and reinterpret_cast it to the correct type. 
			// This is safe because we know the type is correct.
			// We use runtime assertion because component communication can be easily checked
			// in development.
			auto it = components_.find( type_utility::type_hash<component_t>( ) );
			assert( it != components_.end( ) && "Component not found!" );
			return reinterpret_cast<component_t&>( *it->second.get( ) );
		}

		template <typename component_t>
		component_t& get_components( ) const
		{
			throw std::runtime_error( "Not implemented!" );
		}

		void remove_component( BaseComponent& component );

		[[nodiscard]] std::vector<GameObject*>& get_children( );
		[[nodiscard]] const std::vector<GameObject*>& get_children( ) const;
		void collect_children( std::vector<GameObject*>& children );

	private:
		std::unique_ptr<GameObjectView> view_ptr_{};
		GameObject* parent_ptr_{ nullptr };

		Transform local_transform_{};
		Transform world_transform_{};
		bool transform_dirty_{ false };

		std::unordered_multimap<uint64_t, std::unique_ptr<BaseComponent>> components_{};
		Deleter<BaseComponent> deleter_{};

		std::vector<GameObject*> children_{};

		[[nodiscard]] bool is_child( GameObject* pGameObject ) const;
		void add_child( GameObject* pGameObject );
		void remove_child( GameObject* pGameObject );
		[[nodiscard]] bool has_children( ) const;

		void set_transform_dirty( )
		{
			transform_dirty_ = true;

			// optimize this
			for ( auto pChild : children_ )
			{
				pChild->set_transform_dirty( );
			}
		}
		void update_world_transform( );

	};
}

#endif // GAMEOBJECT_H
