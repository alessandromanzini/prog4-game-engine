#ifndef COMPONENT_H
#define COMPONENT_H

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <framework/Deletable.h>
#include <framework/GameObject.h>
// #include <framework/GameObjectView.h>


namespace engine
{
    class Component : public Deletable
    {
    public:
        using owner_t = GameObject;

        ~Component( ) override = default;

        Component( const Component& )                = delete;
        Component( Component&& ) noexcept            = delete;
        Component& operator=( const Component& )     = delete;
        Component& operator=( Component&& ) noexcept = delete;

        virtual void fixed_update( ) { }

        virtual void update( ) { }

        virtual void render( ) const { }


        void mark_for_deletion( ) final
        {
            Deletable::mark_for_deletion( );
            get_owner( ).remove_component( *this );
        }

    protected:
        explicit Component( owner_t& owner )
            : owner_ref_{ owner } { }


        [[nodiscard]] owner_t& get_owner( ) const
        {
            return owner_ref_;
        }

    private:
        owner_t& owner_ref_;

    };

    // +--------------------------------+
    // | CONCEPTS						|
    // +--------------------------------+
    template <typename derived_t, typename... args_t>
    concept DerivedComponentWithBaseContructor =
            std::derived_from<derived_t, Component> and
            std::constructible_from<derived_t, Component::owner_t&, args_t...>;

    template <typename derived_t>
    concept DerivedComponent =
            std::derived_from<derived_t, Component>;

}

#endif //!COMPONENT_H
