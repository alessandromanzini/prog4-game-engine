#ifndef OBJECT_STATS_H
#define OBJECT_STATS_H

#include <component/BubbleCaptureComponent.h>
#include <component/CharacterComponent.h>


namespace game::stats
{
    float get_character_speed( );
    float get_character_jump_force( );

    float get_zen_chan_speed( );
    float get_zen_chan_jump_force( );

    float get_maita_speed( );
    float get_maita_jump_force( );

    CharacterResources get_bub_resources( );
    CharacterResources get_bob_resources( );

    CharacterResources get_zen_chan_resources( );
    BubbleCaptureResources get_zen_chan_capture_resources( );

    CharacterResources get_maita_resources( );
    BubbleCaptureResources get_maita_capture_resources( );
}


#endif //!OBJECT_STATS_H
