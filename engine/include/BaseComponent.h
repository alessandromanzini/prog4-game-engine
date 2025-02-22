#ifndef BASECOMPONENT_H
#define BASECOMPONENT_H

// +--------------------------------+
// | Project Headers				|
// +--------------------------------+
#include "Transform.h"
#include "GameObjectView.h"

// +--------------------------------+
// | Standard Headers				|
// +--------------------------------+
#include <memory>

namespace engine
{
	class BaseComponent
	{
	public:
		virtual ~BaseComponent( ) noexcept = default;

		BaseComponent( const BaseComponent& ) = delete;
		BaseComponent( BaseComponent&& ) noexcept = delete;
		BaseComponent& operator=( const BaseComponent& ) = delete;
		BaseComponent& operator=( BaseComponent&& ) noexcept = delete;

		virtual void fixed_update( )
		{
		};

		virtual void update( )
		{
		};

		virtual void render( ) const
		{
		};

	protected:
		BaseComponent( GameObjectView&& owner )
			: owner_{ std::move( owner ) }
		{
		}

		GameObjectView& get_owner( )
		{
			return owner_;
		}
		const GameObjectView& get_owner( ) const
		{
			return owner_;
		}

	private:
		GameObjectView owner_;

		bool marked_for_deletion_{ false };

	};
}

#endif // !BASECOMPONENT_H
