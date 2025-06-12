#include "JoinMenuController.h"

#include <component/JoinMenuComponent.h>
#include <registration/input.h>


namespace game
{
    JoinMenuController::JoinMenuController( JoinMenuComponent* joinComponent, const engine::binding::DeviceType deviceType )
        : join_menu_component_ptr_{ joinComponent }
    {
        try_register_device( deviceType );
    }


    void JoinMenuController::device_registered( engine::InputMappingContext& context,
                                                const engine::binding::DeviceInfo deviceInfo )
    {
        device_info_ = deviceInfo;
        context.bind_to_input_action( this, engine::UID( IA::JOIN ), &JoinMenuController::join );
        context.bind_to_input_action( this, engine::UID( IA::LEAVE ), &JoinMenuController::leave );
        context.bind_to_input_action( this, engine::UID( IA::MODE_SELECTION ), &JoinMenuController::select );
        context.bind_to_input_action( this, engine::UID( IA::CONFIRM ), &JoinMenuController::confirm );
    }


    void JoinMenuController::join( )
    {
        has_joined_ = join_menu_component_ptr_->join( device_info_ );
    }


    void JoinMenuController::leave( )
    {
        join_menu_component_ptr_->leave( device_info_.id );
        has_joined_ = false;
    }


    void JoinMenuController::select( const float direction ) const
    {
        if ( not has_joined_ )
        {
            return;
        }

        if ( direction < 0.0f )
        {
            join_menu_component_ptr_->prev_selection( );
        }
        else
        {
            join_menu_component_ptr_->next_selection( );
        }
    }


    void JoinMenuController::confirm( ) const
    {
        if ( not has_joined_ )
        {
            return;
        }

        join_menu_component_ptr_->confirm_selection();
    }

}
