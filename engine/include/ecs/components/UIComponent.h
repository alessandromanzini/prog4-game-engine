#ifndef UICOMPONENT_H
#define UICOMPONENT_H

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
	class UIComponent : public Deletable
	{
	public:
		virtual ~UIComponent( ) noexcept = default;

		UIComponent( const UIComponent& ) = delete;
		UIComponent( UIComponent&& ) noexcept = delete;
		UIComponent& operator=( const UIComponent& ) = delete;
		UIComponent& operator=( UIComponent&& ) noexcept = delete;

		virtual void update( )
		{
		}

		// UI needs the render to be non-const
		virtual void render( )
		{
		}

		void mark_for_deletion( ) override final
		{
			Deletable::mark_for_deletion( );
			get_owner( ).remove_ui_component( *this );
		}

	protected:
		UIComponent( UIController& owner )
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
		std::derived_from<derived_t, UIComponent> and
		std::constructible_from<derived_t, UIController&, args_t...>;

	template <typename derived_t>
	concept DerivedUIComponent =
		std::derived_from<derived_t, UIComponent>;

}

#endif // BASEUICOMPONENT_H
