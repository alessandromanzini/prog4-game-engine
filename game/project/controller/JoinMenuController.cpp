#include "JoinMenuController.h"

#include <component/JoinMenuComponent.h>
#include <registration/input.h>


namespace game
{
    JoinMenuController::JoinMenuController( JoinMenuComponent* joinComponent, const engine::binding::DeviceType deviceType )
        : join_menu_component_ptr_{ joinComponent }
    {
        try_register_device( deviceType, std::chrono::milliseconds( 20000 ) );
    }


    void JoinMenuController::device_registered( engine::InputMappingContext& context, const engine::binding::DeviceInfo deviceInfo )
    {
        device_info_ = deviceInfo;
        context.bind_to_input_action( this, engine::UID( IA::JOIN ), &JoinMenuController::join );
        context.bind_to_input_action( this, engine::UID( IA::LEAVE ), &JoinMenuController::leave );
    }


    void JoinMenuController::join( ) const
    {
        join_menu_component_ptr_->join( device_info_ );
    }


    void JoinMenuController::leave( ) const
    {
        join_menu_component_ptr_->leave( device_info_.id );
    }

}
