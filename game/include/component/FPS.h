#ifndef FPS_H
#define FPS_H

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <framework/component/Component.h>


namespace game
{
    // ReSharper disable once CppInconsistentNaming
    class FPS final : public engine::Component
    {
    public:
        explicit FPS( owner_t& owner );

        void tick( ) override;

    private:
        constexpr static float TIME_DELAY_{ 1.f };

        float time_accumulator_{ 0.f };

    };
}

#endif //!FPS_H
