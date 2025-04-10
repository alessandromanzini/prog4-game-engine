#include <components/FPS.h>

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <components/TextComponent.h>
#include <singletons/GameTime.h>


namespace engine
{
    FPS::FPS( owner_t& owner )
        : Component( owner ) { }


    void FPS::update( )
    {
        time_accumulator_ += GAME_TIME.get_delta_time( );
        if ( time_accumulator_ >= TIME_DELAY_ )
        {
            time_accumulator_ -= TIME_DELAY_;

            // Update happens once per second and GetComponent works with maps, so we don't need to save the reference.
            auto& textComponent = get_owner( ).get_component<TextComponent>( );
            textComponent.set_text( std::format( "{:.1f} FPS", GAME_TIME.get_fps( ) ) );
        }
    }

}
