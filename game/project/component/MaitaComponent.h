#ifndef MAITACOMPONENT_H
#define MAITACOMPONENT_H

#include <component/EnemyAiComponent.h>


namespace game
{
    class MaitaComponent final : public EnemyAiComponent
    {
    public:
        explicit MaitaComponent( owner_t& owner );

    private:
        static constexpr glm::vec2 SHOOT_RANGE_{ 100.f, 30.f };
        static constexpr glm::vec2 COMMIT_RANGE_{ 1000.f, 60.f };
        glm::vec2 steering_{};

        glm::vec2 calculate_steering( glm::vec2 targetPosition ) override;

    };

}



#endif //!MAITACOMPONENT_H
