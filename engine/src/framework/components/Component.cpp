#include <framework/components/Component.h>

#include <framework/GameObject.h>


namespace engine
{
    void Component::mark_for_deletion( )
    {
        Deletable::mark_for_deletion( );
        get_owner( ).remove_component( *this );
    }


    Component::Component( owner_t& owner )
        : owner_ref_{ owner } { }


    Component::owner_t& Component::get_owner( ) const
    {
        return owner_ref_;
    }

}
