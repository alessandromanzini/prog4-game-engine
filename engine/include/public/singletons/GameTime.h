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
#include <list>


namespace engine
{
    namespace time
    {
        enum class TimingType
        {
            DELTA_TIME,
            FIXED_DELTA_TIME
        };

        template <typename span_t>
        struct TimeSpan
        {
            const span_t max;
            span_t current;
        };
    }

    class GameTime final : public Singleton<GameTime>
    {
        friend class Singleton;

        template <typename return_t>
        using timed_callback_t = std::pair<time::TimeSpan<float>, std::function<return_t( )>>;

    public:
        void tick( );
        void fixed_tick( );
        void reset( );

        void set_timing_type( time::TimingType type );

        [[nodiscard]] float get_delta_time( ) const;
        [[nodiscard]] float get_fps( ) const;

        [[nodiscard]] bool get_required_fixed_update( ) const;
        [[nodiscard]] std::chrono::nanoseconds get_sleep_time( ) const;

        /**
         * Register a callback to be repeated every amount seconds until the callback return true.
         * @param seconds Interval in seconds between calls
         * @param callback
         */
        void set_interval( float seconds, std::function<bool( )>&& callback );

        /**
         * Register a callback to be repeated every amount seconds indefinitely.
         * @param seconds Interval in seconds between calls
         * @param callback
         */
        void set_interval( float seconds, const std::function<void( )>& callback );

        /**
         * Register a callback to be called once after the amount of seconds.
         * @param seconds Interval in seconds before the call
         * @param callback
         */
        void set_timeout( float seconds, std::function<void( )>&& callback );

    private:
        static constexpr int MS_PER_FRAME_{ 16 };
        static constexpr float FIXED_TIME_STEP_{ 0.005f };

        float delta_time_{ 0.f };
        const float* current_delta_ptr_{ &delta_time_ };

        std::chrono::high_resolution_clock::time_point last_time_{};
        float lag_{ 0.f };

        std::list<timed_callback_t<bool>> intervals_{};
        std::list<timed_callback_t<void>> timeouts_{};

        GameTime( ) = default;

        template <typename callback_return_t>
        void handle_callbacks( std::list<timed_callback_t<callback_return_t>>& callbacks, const bool discardFinished ) const;

    };


    template <typename callback_return_t>
    void GameTime::handle_callbacks( std::list<timed_callback_t<callback_return_t>>& callbacks,
                                     const bool discardFinished ) const
    {
        for ( auto& [span, callback] : callbacks )
        {
            span.current -= delta_time_;

            if ( span.current <= 0.f )
            {
                bool shouldReset = false;

                if constexpr ( std::is_void_v<callback_return_t> )
                {
                    callback( );
                    shouldReset = !discardFinished;
                }
                else
                {
                    shouldReset = !discardFinished || callback( );
                }

                if ( shouldReset )
                {
                    span.current = span.max;
                }
            }
        }

        std::erase_if( callbacks, []( auto& pair )
                           {
                               return pair.first.current <= 0.f;
                           } );
    }


    extern GameTime& GAME_TIME;

}

#endif // GAMETIME_H
