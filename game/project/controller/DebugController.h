#ifndef DEBUGCONTROLLER_H
#define DEBUGCONTROLLER_H

#include <framework/controller/PlayerController.h>


namespace game
{
    class DebugController final : public engine::PlayerController
    {
    public:
        DebugController( );

    private:
        void device_registered( engine::InputMappingContext& context, engine::binding::DeviceInfo deviceInfo ) override;

        void skip_level( ) const;
        void toggle_mute( ) const;
        void quit( ) const;

    };

}


#endif //!DEBUGCONTROLLER_H
