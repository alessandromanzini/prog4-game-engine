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

		Transform& get_transform( ) const;

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
