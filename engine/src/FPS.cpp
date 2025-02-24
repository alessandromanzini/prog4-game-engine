#include "FPS.h"

// +--------------------------------+
// | Project Headers				|
// +--------------------------------+
#include "TextComponent.h"
#include "GameTime.h"

// +--------------------------------+
// | Standard Headers				|
// +--------------------------------+
#include <sstream>
#include <format>

using namespace engine;

FPS::FPS( GameObjectView& owner )
	: BaseComponent( owner )
{

}

void FPS::update( )
{
	time_accumulator_ += GameTime::get_instance( ).get_delta_time( );
	if ( time_accumulator_ >= time_delay_ )
	{
		time_accumulator_ -= time_delay_;

		// Update happens once per second and GetComponent works with maps, so we don't need to save the reference.
		auto& textComponent = get_owner( ).get_component<TextComponent>( );
		textComponent.set_text( std::format( "{:.1f} FPS", GameTime::get_instance( ).get_fps( ) ) );
	}
}
