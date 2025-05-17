#ifndef BOXCOLLIDERCOMPONENT_H
#define BOXCOLLIDERCOMPONENT_H

#include <framework/component/physics/ColliderComponent.h>

#include <array>


namespace engine
{
    class BoxColliderComponent final : public ColliderComponent
    {
    public:
        explicit BoxColliderComponent( owner_t& owner, glm::vec2 size );

        [[nodiscard]] bool hit_test( glm::vec2 ) const override;

    private:
        std::array<glm::vec2, 4> pivots_{};

        [[nodiscard]] std::span<const glm::vec2> get_pivots( ) const override;

    };

}


#endif //!BOXCOLLIDERCOMPONENT_H
