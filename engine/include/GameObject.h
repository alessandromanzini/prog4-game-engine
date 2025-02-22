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
#include "Deletable.h"
#include "Transform.h"
#include "type_utility.hpp"

namespace engine
{
	class BaseComponent;
	class GameObjectView;
	class GameObject final : public Deletable
	{
	public:
		GameObject( ) = default;
		~GameObject( ) noexcept = default;

		GameObject( const GameObject& other ) = delete;
		GameObject( GameObject&& other ) noexcept = delete;
		GameObject& operator=( const GameObject& other ) = delete;
		GameObject& operator=( GameObject&& other ) noexcept = delete;

		void fixed_update( );
		void update( );
		void render( ) const;

		Transform& get_transform( );

		template <typename component_t, typename... args_t>
		void add_component( args_t&&... args )
		{
			// Initializing component with GameObjectView and arguments' perfect forwarding.
			// We insert the element in the multimap with its compile-time type hash as the key.
			auto component = std::make_unique<component_t>( GameObjectView{ *this }, std::forward<args_t>( args )... );
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

	private:
		Transform transform_{};
		std::unordered_multimap<uint64_t, std::unique_ptr<BaseComponent>> components_{};

	};
}

#endif // GAMEOBJECT_H
