#ifndef GAMETIME_H
#define GAMETIME_H

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <singletons/Singleton.h>

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <chrono>


namespace engine
{
    enum class TimingType
    {
        DELTA_TIME,
        FIXED_DELTA_TIME
    };

    class GameTime final : public Singleton<GameTime>
    {
        friend class Singleton;
    public:
        ~GameTime( ) noexcept override = default;

        GameTime( const GameTime& )                = delete;
        GameTime( GameTime&& ) noexcept            = delete;
        GameTime& operator=( const GameTime& )     = delete;
        GameTime& operator=( GameTime&& ) noexcept = delete;

        void tick( );
        void fixed_tick( );
        void reset( );

        void set_timing_type( TimingType type );

        [[nodiscard]] float get_delta_time( ) const;
        [[nodiscard]] float get_fps( ) const;

        [[nodiscard]] bool get_required_fixed_update( ) const;
        [[nodiscard]] std::chrono::nanoseconds get_sleep_time( ) const;

    private:
        static constexpr int MS_PER_FRAME_{ 16 };
        static constexpr float FIXED_TIME_STEP_{ 0.005f };

        float delta_time_{ 0.f };
        const float* current_delta_ptr_{ &delta_time_ };

        std::chrono::high_resolution_clock::time_point last_time_{};
        float lag_{ 0.f };

        GameTime( )                    = default;

    };

    extern GameTime& GAME_TIME;

}

#endif // GAMETIME_H
