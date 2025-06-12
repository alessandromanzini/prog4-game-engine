#include "JoinMenuComponent.h"

#include <controller/CharacterController.h>
#include <controller/JoinMenuController.h>
#include <framework/component/TextureComponent.h>
#include <registration/object_initializers.h>
#include <singleton/GameInstance.h>
#include <singleton/ScenePool.h>


namespace game
{
    JoinMenuComponent::JoinMenuComponent( owner_t& owner, engine::GameObject* selectionObject,
                                          std::vector<engine::GameObject*>&& playerObjects )
        : Component{ owner }
        , selection_object_ptr_{ selectionObject }
        , selector_arrow_ref_{ get_owner(  ).create_child(  ) }
        , player_objects_{ std::move( playerObjects ) }
    {
        engine::GAME_INSTANCE.add_controller<JoinMenuController>( this, engine::binding::DeviceType::KEYBOARD );
        engine::GAME_INSTANCE.add_controller<JoinMenuController>( this, engine::binding::DeviceType::GAMEPAD );
        engine::GAME_INSTANCE.add_controller<JoinMenuController>( this, engine::binding::DeviceType::GAMEPAD );

        for ( const auto object : player_objects_ )
        {
            arrange_ui( object, std::nullopt );
        }

        selector_arrow_ref_.add_component<engine::TextureComponent>( "icons/selector.png" );
        move_selector( selection_object_ptr_->get_children( )[0] );
    }


    bool JoinMenuComponent::join( engine::binding::DeviceInfo device )
    {
        bool found{ false };
        size_t emptyIndex{};
        for ( size_t i{}; i < slot_devices_.size( ); ++i )
        {
            if ( not slot_devices_[i].has_value( ) )
            {
                if ( not found )
                {
                    emptyIndex = i;
                    found = true;
                }
            }
            else if ( slot_devices_[i]->id == device.id )
            {
                // Device already registered
                return true;
            }
        }

        if ( not found )
        {
            return false;
        }
        slot_devices_[emptyIndex] = device;
        arrange_ui( player_objects_[emptyIndex], device );
        return true;
    }


    void JoinMenuComponent::leave( const uint8_t id )
    {
        for ( size_t i{}; i < slot_devices_.size( ); ++i )
        {
            if ( slot_devices_[i].has_value( ) && slot_devices_[i]->id == id )
            {
                slot_devices_[i] = std::nullopt;
                arrange_ui( player_objects_[i], std::nullopt );
                return;
            }
        }
    }


    void JoinMenuComponent::next_selection( )
    {
        const auto& children = selection_object_ptr_->get_children( );
        selection_index_ = ( selection_index_ + 1 ) % children.size( );
        move_selector( children[selection_index_] );
    }


    void JoinMenuComponent::prev_selection( )
    {
        const auto& children = selection_object_ptr_->get_children( );
        selection_index_ = ( selection_index_ - 1 ) % children.size( );
        move_selector( children[selection_index_] );
    }


    void JoinMenuComponent::confirm_selection( )
    {
        engine::SCENE_POOL.unload_scene( engine::SCENE_POOL.get_active_scene( ).get_name( ) );
        auto& scene = engine::SCENE_POOL.create_scene( "game" );

        engine::GAME_INSTANCE.clear_controllers( );

        // Bub
        auto& bub = scene.create_object( );
        create_bub( bub, { 200.f, 375.f } );

        auto& zenchan = scene.create_object( );
        create_zenchan( zenchan, { 300.f, 375.f }, { &bub } );

        auto& grid = scene.create_object( );
        create_grid( grid );

        // auto& controller = engine::GAME_INSTANCE.add_controller<CharacterController>( );
        // controller.possess( &bub );
    }


    void JoinMenuComponent::arrange_ui( engine::GameObject* object, const std::optional<engine::binding::DeviceInfo>& info ) const
    {
        if ( not info.has_value( ) )
        {
            object->set_visibility( false );
            return;
        }

        object->set_visibility( true );
        for ( const auto child : object->get_children(  ) )
        {
            child->mark_for_deletion( );
        }

        auto& texture = object->create_child( );
        texture.set_local_transform( engine::Transform::from_translation( { 45.f, 5.f } ) );
        if ( info->type == engine::binding::DeviceType::KEYBOARD )
        {
            texture.add_component<engine::TextureComponent>( "icons/keyboard.png" );
        }
        else if ( info->type == engine::binding::DeviceType::GAMEPAD )
        {
            texture.add_component<engine::TextureComponent>( "icons/gamepad.png" );
        }
    }


    void JoinMenuComponent::move_selector( engine::GameObject* target ) const
    {
        const auto targetPos = target->get_world_transform( ).get_position( );
        selector_arrow_ref_.set_world_transform( engine::Transform::from_translation( targetPos - glm::vec2{ 20.f, -15.f } ) );
    }


}
