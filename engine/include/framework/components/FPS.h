#ifndef FPS_H
#define FPS_H

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <Component.h>


namespace engine
{
    // ReSharper disable once CppInconsistentNaming
    class FPS final : public Component
    {
    public:
        explicit FPS( owner_t& owner );
        ~FPS( ) override = default;

        FPS( const FPS& )                = delete;
        FPS( FPS&& ) noexcept            = delete;
        FPS& operator=( const FPS& )     = delete;
        FPS& operator=( FPS&& ) noexcept = delete;

        void update( ) override;

    private:
        constexpr static float TIME_DELAY_{ 1.f };

        float time_accumulator_{ 0.f };

    };
}

#endif //!FPS_H
