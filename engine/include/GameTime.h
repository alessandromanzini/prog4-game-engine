#ifndef GAMETIME_H
#define GAMETIME_H

// +--------------------------------+
// | Project Includes				|
// +--------------------------------+
#include "Singleton.h"

// +--------------------------------+
// | Standard Includes				|
// +--------------------------------+
#include <chrono>

namespace engine
{
	class GameTime final : public Singleton<GameTime>
	{
	public:
		GameTime( ) = default;
		~GameTime( ) noexcept = default;

		GameTime( const GameTime& ) = delete;
		GameTime( GameTime&& ) noexcept = delete;
		GameTime& operator=( const GameTime& ) = delete;
		GameTime& operator=( GameTime&& ) noexcept = delete;

		void tick( );
		void fixed_tick( );
		void reset( ) noexcept;

		float get_delta_time( ) const noexcept;
		float get_fps( ) const;

		bool get_required_fixed_update( ) const noexcept;
		std::chrono::nanoseconds get_sleep_time( ) const noexcept;

	private:
		static constexpr int ms_per_frame_{ 16 };
		static constexpr float fixed_time_step_{ 0.005f };

		float delta_time_{ 0.f };

		std::chrono::high_resolution_clock::time_point last_time_{};
		float lag_{ 0.f };

	};

}

#endif // GAMETIME_H
