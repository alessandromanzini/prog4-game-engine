#include <singletons/GameTime.h>


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

        handle_callbacks( timeouts_, true );
        handle_callbacks( intervals_, false );
    }


    void GameTime::fixed_tick( )
    {
        lag_ -= FIXED_TIME_STEP_;
    }


    void GameTime::reset( )
    {
        last_time_ = high_resolution_clock::now( );
        lag_       = 0.f;
    }


    float GameTime::get_delta_time( ) const
    {
        return *current_delta_ptr_;
    }


    float GameTime::get_fps( ) const
    {
        const auto fps{ 1.f / delta_time_ };
        return fps;
    }


    bool GameTime::get_required_fixed_update( ) const
    {
        return lag_ >= FIXED_TIME_STEP_;
    }


    nanoseconds GameTime::get_sleep_time( ) const
    {
        return last_time_ + milliseconds( MS_PER_FRAME_ ) - high_resolution_clock::now( );
    }


    void GameTime::set_interval( const float seconds, std::function<bool( )>&& callback )
    {
        intervals_.emplace_back( std::make_pair( time::TimeSpan{ seconds, seconds }, std::move( callback ) ) );
    }


    void GameTime::set_interval( const float seconds, const std::function<void( )>& callback )
    {
        set_interval( seconds, std::function{
                          [callback]( ) -> bool
                              {
                                  callback( );
                                  return false;
                              }
                      } );
    }


    void GameTime::set_timeout( const float seconds, std::function<void( )>&& callback )
    {
        timeouts_.emplace_back( std::make_pair( time::TimeSpan{ seconds, seconds }, std::move( callback ) ) );
    }


    void GameTime::set_timing_type( const time::TimingType type )
    {
        switch ( type )
        {
            case time::TimingType::DELTA_TIME:
                current_delta_ptr_ = &delta_time_;
                break;
            case time::TimingType::FIXED_DELTA_TIME:
                current_delta_ptr_ = &FIXED_TIME_STEP_;
                break;
        }
    }
}
