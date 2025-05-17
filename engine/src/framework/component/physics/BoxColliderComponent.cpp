#include <framework/GameObject.h>
#include <framework/component/physics/BoxColliderComponent.h>


namespace engine
{
    BoxColliderComponent::BoxColliderComponent( owner_t& owner, const glm::vec2 size )
        : ColliderComponent{ owner }
        , pivots_{
            glm::vec2{ 0.f, 0.f },
            glm::vec2{ size.x, 0.f },
            glm::vec2{ size.x, size.y },
            glm::vec2{ 0.f, size.y }
        } { }


    bool BoxColliderComponent::hit_test( const glm::vec2 point ) const
    {
        const auto location = get_offset( ) + get_owner( ).get_world_transform( ).get_position( );
        return point.x >= pivots_[0].x + location.x && point.x <= pivots_[2].x + location.x &&
               point.y >= pivots_[0].y + location.y && point.y <= pivots_[2].y + location.y;
    }


    std::span<const glm::vec2> BoxColliderComponent::get_pivots( ) const
    {
        return std::span<const glm::vec2>{ pivots_ };
    }

}
