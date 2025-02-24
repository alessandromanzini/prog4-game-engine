#ifndef BASECOMPONENT_H
#define BASECOMPONENT_H

// +--------------------------------+
// | Project Headers				|
// +--------------------------------+
#include "Transform.h"
#include "GameObjectView.h"
#include "Deletable.h"

// +--------------------------------+
// | Standard Headers				|
// +--------------------------------+
#include <memory>

namespace engine
{
	class BaseComponent : public Deletable
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

		void mark_for_deletion( ) override
		{
			Deletable::mark_for_deletion( );
			get_owner( ).remove_component( *this );
		}

	protected:
		BaseComponent( GameObjectView& owner )
			: owner_ref_{ owner }
		{
		}

		GameObjectView& get_owner( ) const
		{
			return owner_ref_;
		}

	private:
		GameObjectView& owner_ref_;

	};

}

#endif // !BASECOMPONENT_H
