#ifndef ROTATINGCOMPONENT_H
#define ROTATINGCOMPONENT_H

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <framework/components/Component.h>


namespace game
{
    class RotatingComponent final : public engine::Component
    {
    public:
        RotatingComponent( owner_t& owner, float rotationSpeed );
        ~RotatingComponent( ) override = default;

        RotatingComponent( const RotatingComponent& )                = delete;
        RotatingComponent( RotatingComponent&& ) noexcept            = delete;
        RotatingComponent& operator=( const RotatingComponent& )     = delete;
        RotatingComponent& operator=( RotatingComponent&& ) noexcept = delete;

        void tick( ) override;

    private:
        float rotation_speed_{};

    };
}


#endif //!ROTATINGCOMPONENT_H
