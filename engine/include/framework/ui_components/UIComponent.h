#ifndef UICOMPONENT_H
#define UICOMPONENT_H

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <Deletable.h>
#include <singletons/UIController.h>


namespace engine
{
    // ReSharper disable once CppInconsistentNaming
    class UIComponent : public Deletable
    {
    public:
        ~UIComponent( ) override = default;

        UIComponent( const UIComponent& )                = delete;
        UIComponent( UIComponent&& ) noexcept            = delete;
        UIComponent& operator=( const UIComponent& )     = delete;
        UIComponent& operator=( UIComponent&& ) noexcept = delete;

        virtual void update( ) { }

        // UI needs the render to be non-const
        virtual void render( ) { }


        void mark_for_deletion( ) final
        {
            Deletable::mark_for_deletion( );
            get_owner( ).remove_ui_component( *this );
        }

    protected:
        explicit UIComponent( UIController& owner )
            : owner_ref_{ owner } { }


        [[nodiscard]] UIController& get_owner( ) const
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
