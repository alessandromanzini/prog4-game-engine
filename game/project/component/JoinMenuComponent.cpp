#include "JoinMenuComponent.h"

#include <controller/JoinMenuController.h>
#include <framework/component/TextureComponent.h>
#include <singleton/GameInstance.h>


namespace game
{
    JoinMenuComponent::JoinMenuComponent( owner_t& owner, engine::GameObject* selectionObject,
                                          std::vector<engine::GameObject*>&& playerObjects )
        : Component{ owner }
        , selection_object_ptr_{ selectionObject }
        , player_objects_{ std::move( playerObjects ) }
    {
        engine::GAME_INSTANCE.add_controller<JoinMenuController>( this, engine::binding::DeviceType::KEYBOARD );
        engine::GAME_INSTANCE.add_controller<JoinMenuController>( this, engine::binding::DeviceType::GAMEPAD );
        engine::GAME_INSTANCE.add_controller<JoinMenuController>( this, engine::binding::DeviceType::GAMEPAD );
    }


    void JoinMenuComponent::join( engine::binding::DeviceInfo device )
    {
        bool assigned{ false };
        for ( size_t i{}; i < slot_devices_.size( ); ++i )
        {
            if ( not slot_devices_[i].has_value( ) )
            {
                if ( assigned )
                {
                    player_objects_[i]->set_visibility( false );
                    continue;
                }
                slot_devices_[i] = device;
                assigned = true;
            }
            arrange_ui( player_objects_[i], slot_devices_[i].value( ) );
        }
    }


    void JoinMenuComponent::leave( const uint8_t id )
    {
        for ( auto& slot : slot_devices_ )
        {
            if ( slot.has_value( ) && slot->id == id )
            {
                slot = std::nullopt;
                return;
            }
        }
    }


    void JoinMenuComponent::arrange_ui( engine::GameObject* object, const engine::binding::DeviceInfo& info ) const
    {
        object->set_visibility( true );
        if ( const auto texture = object->get_component<engine::TextureComponent>(  ); texture.has_value(  ) )
        {
            texture.value( ).mark_for_deletion(  );
        }

        if ( info.type == engine::binding::DeviceType::KEYBOARD )
        {
            object->add_component<engine::TextureComponent>( "icons/keyboard.png" );
        }
        else if ( info.type == engine::binding::DeviceType::GAMEPAD )
        {
            object->add_component<engine::TextureComponent>( "icons/gamepad.png" );
        }
    }


}
