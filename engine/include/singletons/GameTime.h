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
	enum class TimingType
	{
		DELTA_TIME,
		FIXED_DELTA_TIME
	};

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

		void set_timing_type( TimingType type );

		[[nodiscard]] float get_delta_time( ) const noexcept;
		[[nodiscard]] float get_fps( ) const;

		[[nodiscard]] bool get_required_fixed_update( ) const noexcept;
		[[nodiscard]] std::chrono::nanoseconds get_sleep_time( ) const;

	private:
		static constexpr int ms_per_frame_{ 16 };
		static constexpr float fixed_time_step_{ 0.005f };

		float delta_time_{ 0.f };
		const float* current_delta_ptr_{ &delta_time_ };

		std::chrono::high_resolution_clock::time_point last_time_{};
		float lag_{ 0.f };

	};

	extern GameTime& GAME_TIME;

}

#endif // GAMETIME_H
