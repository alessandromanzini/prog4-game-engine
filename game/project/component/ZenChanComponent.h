#ifndef ZENCHANCOMPONENT_H
#define ZENCHANCOMPONENT_H

#include <component/EnemyAiComponent.h>


namespace game
{
    class ZenChanComponent final : public EnemyAiComponent
    {
    public:
        explicit ZenChanComponent( owner_t& owner );

    private:
        glm::vec2 steering_{};

        glm::vec2 calculate_steering( glm::vec2 targetPosition ) override;

    };

}


#endif //!ZENCHANCOMPONENT_H
