#include "JoinMenuComponent.h"

#include <component/ScoreComponent.h>
#include <controller/CharacterController.h>
#include <framework/component/TextureComponent.h>
#include <registration/object_initializers.h>
#include <registration/tags.h>
#include <singleton/GameInstance.h>
#include <singleton/ResourceManager.h>
#include <singleton/ScenePool.h>
#include <singleton/ServiceLocator.h>

#include "LevelLoaderComponent.h"


namespace game
{
    JoinMenuComponent::JoinMenuComponent( owner_t& owner, engine::GameObject* selectionObject,
                                          std::vector<engine::GameObject*>&& playerObjects )
        : Component{ owner }
        , selection_object_ptr_{ selectionObject }
        , selector_arrow_ref_{ get_owner(  ).create_child(  ) }
        , player_objects_{ std::move( playerObjects ) }
    {
        player_controllers_.emplace_back( &engine::GAME_INSTANCE.add_controller<CharacterController>( this, engine::binding::DeviceType::KEYBOARD ) );
        player_controllers_.emplace_back( &engine::GAME_INSTANCE.add_controller<CharacterController>( this, engine::binding::DeviceType::GAMEPAD ) );
        player_controllers_.emplace_back( &engine::GAME_INSTANCE.add_controller<CharacterController>( this, engine::binding::DeviceType::GAMEPAD ) );

        for ( const auto object : player_objects_ )
        {
            arrange_ui( object, std::nullopt );
        }

        selector_arrow_ref_.add_component<engine::TextureComponent>( "icons/selector.png" );
        move_selector( selection_object_ptr_->get_children( )[0] );
    }


    void JoinMenuComponent::tick( )
    {
        for ( const auto controller : player_controllers_ )
        {
            controller->set_block_selection( false );
            controller->set_gameover_component( nullptr );
        }
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


    void JoinMenuComponent::confirm_selection( ) const
    {
        const auto font = engine::RESOURCE_MANAGER.load_font( "fonts/pixelify.ttf", 36 );

        switch ( selection_object_ptr_->get_children( )[selection_index_]->get_tag( ).uid )
        {
            case engine::UID( ObjectTags::ARCADE ).uid:
            {
                auto& scene = engine::SCENE_POOL.create_scene( "game" );
                engine::SCENE_POOL.select_scene( "game" );
                create_arcade( scene, player_controllers_, font );
                break;
            }

            case engine::UID( ObjectTags::VERSUS ).uid:
                if ( std::ranges::count_if( player_controllers_, []( auto controller ){ return controller->has_joined(  ); } ) >= 2 )
                {
                    auto& scene = engine::SCENE_POOL.create_scene( "game" );
                    engine::SCENE_POOL.select_scene( "game" );
                    create_versus( scene, player_controllers_, font );
                }
                break;

            default:
                return;
        }
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
