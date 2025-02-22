#ifndef FPS_H
#define FPS_H

#include "BaseComponent.h"

namespace engine
{
	class FPS final : public BaseComponent
	{
	public:
		explicit FPS( GameObjectView&& owner );
		~FPS( ) noexcept = default;

		FPS( const FPS& ) = delete;
		FPS( FPS&& ) noexcept = delete;
		FPS& operator=( const FPS& ) = delete;
		FPS& operator=( FPS&& ) noexcept = delete;

		void update( ) override;

	private:
		constexpr static float time_delay_{ 1.f };

		float time_accumulator_{ 0.f };

	};
}

#endif // !FPS_H
