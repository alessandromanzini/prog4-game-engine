#ifndef GAMEOBJECTVIEW_H
#define GAMEOBJECTVIEW_H

// +--------------------------------+
// | Project Headers				|
// +--------------------------------+
#include "Transform.h"
#include "GameObject.h"

namespace engine
{
	class GameObjectView final
	{
	public:
		~GameObjectView( ) noexcept = default;

		GameObjectView( const GameObjectView& ) = delete;
		GameObjectView( GameObjectView&& ) noexcept = default;
		GameObjectView& operator=( const GameObjectView& ) = delete;
		GameObjectView& operator=( GameObjectView&& ) noexcept = default;

		[[nodiscard]] const Transform& get_world_transform( );
		[[nodiscard]] const Transform& get_local_transform( ) const;
		void set_world_transform( const Transform& transform );
		void set_local_transform( const Transform& transform );
		void set_world_transform( Transform&& transform );
		void set_local_transform( Transform&& transform );

		template <typename component_t>
		component_t& get_component( ) const
		{
			return gameobject_ptr_->get_component<component_t>( );
		}

		friend class GameObject;

	private:
		GameObject* gameobject_ptr_{ nullptr };

		// This class is only callable by the GameObject. It can contain private shared data for future use.
		explicit GameObjectView( GameObject& gameobject );
	};
}

#endif // GAMEOBJECTVIEW_H
