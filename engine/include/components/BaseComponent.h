#ifndef BASECOMPONENT_H
#define BASECOMPONENT_H

// +--------------------------------+
// | Project Headers				|
// +--------------------------------+
#include "GameObjectView.h"
#include "Deletable.h"

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

	// +--------------------------------+
	// | CONCEPTS						|
	// +--------------------------------+
	template <typename derived_t, typename... args_t>
	concept DerivedComponentWithBaseContructor =
		std::derived_from<derived_t, BaseComponent> and
		std::constructible_from<derived_t, GameObjectView&, args_t...>;

	template <typename derived_t>
	concept DerivedComponent =
		std::derived_from<derived_t, BaseComponent>;

}

#endif // !BASECOMPONENT_H
