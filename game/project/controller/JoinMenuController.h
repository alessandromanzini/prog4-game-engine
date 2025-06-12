#ifndef JOINMENUCONTROLLER_H
#define JOINMENUCONTROLLER_H

#include <framework/controller/PlayerController.h>


namespace game
{
    class JoinMenuComponent;
    class JoinMenuController final : public engine::PlayerController
    {
    public:
        explicit JoinMenuController( JoinMenuComponent* joinComponent, engine::binding::DeviceType deviceType );

    private:
        JoinMenuComponent* join_menu_component_ptr_{ nullptr };
        engine::binding::DeviceInfo device_info_{};
        bool has_joined_{ false };

        void device_registered( engine::InputMappingContext& context, engine::binding::DeviceInfo deviceInfo ) override;

        void join( );
        void leave( );
        void select( float direction ) const;
        void confirm( ) const;

    };

}


#endif //!JOINMENUCONTROLLER_H
