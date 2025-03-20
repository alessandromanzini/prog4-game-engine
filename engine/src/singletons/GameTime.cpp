#include "singletons/GameTime.h"


using namespace std::chrono;

namespace engine
{
    GameTime& GAME_TIME = GameTime::get_instance( );


    void GameTime::tick( )
    {
        const auto currentTime = high_resolution_clock::now( );

        delta_time_ = duration<float>( currentTime - last_time_ ).count( );

        last_time_ = currentTime;
        lag_ += delta_time_;
    }


    void GameTime::fixed_tick( )
    {
        lag_ -= fixed_time_step_;
    }


    void GameTime::reset( ) noexcept
    {
        last_time_ = high_resolution_clock::now( );
        lag_       = 0.f;
    }


    float GameTime::get_delta_time( ) const noexcept
    {
        return *current_delta_ptr_;
    }


    float GameTime::get_fps( ) const
    {
        const auto fps{ 1.f / delta_time_ };
        return fps;
    }


    bool GameTime::get_required_fixed_update( ) const noexcept
    {
        return lag_ >= fixed_time_step_;
    }


    nanoseconds GameTime::get_sleep_time( ) const
    {
        return last_time_ + milliseconds( ms_per_frame_ ) - high_resolution_clock::now( );
    }


    void GameTime::set_timing_type( const TimingType type )
    {
        switch ( type )
        {
            case TimingType::DELTA_TIME:
                current_delta_ptr_ = &delta_time_;
                break;
            case TimingType::FIXED_DELTA_TIME:
                current_delta_ptr_ = &fixed_time_step_;
                break;
        }
    }
}
