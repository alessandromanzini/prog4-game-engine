#include <framework/component/physics/ColliderComponent.h>


namespace engine
{
    ColliderComponent::ColliderComponent( owner_t& owner )
        : Component{ owner } { }


    void ColliderComponent::offset_by( const glm::vec2& offset )
    {
        offset_ += offset;
    }


    void ColliderComponent::offset_to( const glm::vec2& offset )
    {
        offset_ = offset;
    }


    bool ColliderComponent::hit_test( const ColliderComponent& other ) const
    {
        for ( auto pivot : get_pivots( ) )
        {
            if ( other.hit_test( pivot + offset_ ) )
            {
                return true;
            }
        }
        return false;
    }

}
