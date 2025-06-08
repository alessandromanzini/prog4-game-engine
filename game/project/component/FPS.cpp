#include <component/FPS.h>

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <../../../engine/include/public/framework/GameObject.h>
#include <../../../engine/include/public/framework/component/TextComponent.h>
#include <../../../engine/include/public/singleton/GameTime.h>


using namespace engine;

namespace game
{
    FPS::FPS( owner_t& owner )
        : Component( owner ) { }


    void FPS::tick( )
    {
        time_accumulator_ += GAME_TIME.get_delta_time( );
        if ( time_accumulator_ >= TIME_DELAY_ )
        {
            time_accumulator_ -= TIME_DELAY_;

            // Update happens once per second and GetComponent works with maps, so we don't need to save the reference.
            get_owner( ).get_component<TextComponent>( ).value(  ).set_text( std::format( "{:.1f} FPS", GAME_TIME.get_fps( ) ) );
        }
    }

}
