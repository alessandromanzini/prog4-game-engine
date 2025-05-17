#ifndef BOXCOLLIDERCOMPONENT_H
#define BOXCOLLIDERCOMPONENT_H

#include <framework/component/physics/ColliderComponent.h>


namespace engine
{
    class BoxColliderComponent final : public ColliderComponent
    {
    public:
        explicit BoxColliderComponent( owner_t& owner )
            : ColliderComponent{ owner } { }


        [[nodiscard]] bool hit_test( const glm::vec2& ) const override { return false; }

    protected:
        [[nodiscard]] std::span<glm::vec2> get_pivots( ) const override
        {
            return std::span<glm::vec2>{};
        }

    private:

    };

}


#endif //!BOXCOLLIDERCOMPONENT_H
