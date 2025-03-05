#include "components/FPS.h"

// +--------------------------------+
// | Project Headers				|
// +--------------------------------+
#include "singletons/GameTime.h"
#include "components/TextComponent.h"

// +--------------------------------+
// | Standard Headers				|
// +--------------------------------+
#include <sstream>
#include <format>

namespace engine
{
	FPS::FPS( GameObjectView& owner )
		: BaseComponent( owner )
	{

	}

	void FPS::update( )
	{
		time_accumulator_ += GAME_TIME.get_delta_time( );
		if ( time_accumulator_ >= time_delay_ )
		{
			time_accumulator_ -= time_delay_;

			// Update happens once per second and GetComponent works with maps, so we don't need to save the reference.
			auto& textComponent = get_owner( ).get_component<TextComponent>( );
			textComponent.set_text( std::format( "{:.1f} FPS", GAME_TIME.get_fps( ) ) );
		}
	}
}
