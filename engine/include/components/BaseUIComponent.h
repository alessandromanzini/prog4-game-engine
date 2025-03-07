#ifndef BASEUICOMPONENT_H
#define BASEUICOMPONENT_H

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include "singletons/UIController.h"
#include "Deletable.h"

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <concepts>

namespace engine
{
	class BaseUIComponent : public Deletable
	{
	public:
		virtual ~BaseUIComponent( ) noexcept = default;

		BaseUIComponent( const BaseUIComponent& ) = delete;
		BaseUIComponent( BaseUIComponent&& ) noexcept = delete;
		BaseUIComponent& operator=( const BaseUIComponent& ) = delete;
		BaseUIComponent& operator=( BaseUIComponent&& ) noexcept = delete;

		virtual void update( )
		{
		};

		// UI needs the render to be non-const
		virtual void render( )
		{
		};

		void mark_for_deletion( ) override
		{
			get_owner( ).remove_ui_component( *this );
		}

	protected:
		BaseUIComponent( UIController& owner )
			: owner_ref_{ owner }
		{
		}

		UIController& get_owner( ) const
		{
			return owner_ref_;
		}

	private:
		UIController& owner_ref_;

	};

	// +--------------------------------+
	// | CONCEPTS						|
	// +--------------------------------+
	template <typename derived_t, typename... args_t>
	concept DerivedUIComponentWithBaseContructor =
		std::derived_from<derived_t, BaseUIComponent> and
		std::constructible_from<derived_t, UIController&, args_t...>;

	template <typename derived_t>
	concept DerivedUIComponent =
		std::derived_from<derived_t, BaseUIComponent>;

}

#endif // BASEUICOMPONENT_H
