#ifndef ROTATINGCOMPONENT_H
#define ROTATINGCOMPONENT_H

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include "components/Component.h"


namespace game
{
    class RotatingComponent final : public engine::Component
    {
    public:
        RotatingComponent( engine::GameObjectView& owner, float rotationSpeed );

        void update( ) override;

    private:
        float rotation_speed_{};

    };
}


#endif //!ROTATINGCOMPONENT_H
