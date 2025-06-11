#include "EnemyAiComponent.h"

#include <component/CharacterComponent.h>
#include <framework/GameObject.h>

#include "BubbleCaptureComponent.h"


namespace game
{
    EnemyAiComponent::EnemyAiComponent( owner_t& owner )
        : Component{ owner }
    {
        character_component_ptr_      = &get_owner( ).get_component<CharacterComponent>( );
        bubble_capture_component_ptr_ = &get_owner( ).get_component<BubbleCaptureComponent>( );
        assert( character_component_ptr_ && bubble_capture_component_ptr_ &&
            "EnemyAiComponent::EnemyAiComponent: missing components" );
    }


    void EnemyAiComponent::tick( )
    {
        if ( bubble_capture_component_ptr_->is_captured(  ) )
        {
            return;
        }

        const auto selfPosition = get_owner( ).get_world_transform( ).get_position( );
        float closestDistance{ std::numeric_limits<float>::max( ) };
        for ( const auto& target : targets_ )
        {
            auto targetPosition = target->get_world_transform( ).get_position( );
            if ( const auto distance = dot( targetPosition - selfPosition, targetPosition - selfPosition );
                distance < closestDistance )
            {
                closestDistance = distance;
                target_ptr_     = target;
            }
        }

        if ( const auto steering = calculate_steering( target_ptr_->get_world_transform( ).get_position( ) );
            dot( steering, steering ) > 0.01f )
        {
            character_component_ptr_->move( normalize( steering ) );
        }
    }


    engine::GameObject* EnemyAiComponent::get_target( ) const
    {
        return target_ptr_;
    }


    void EnemyAiComponent::add_target( engine::GameObject* target )
    {
        targets_.push_back( target );
    }


    void EnemyAiComponent::set_targets( std::vector<engine::GameObject*>&& targets )
    {
        targets_ = std::move( targets );
    }


    CharacterComponent* EnemyAiComponent::get_character_component( ) const
    {
        return character_component_ptr_;
    }

}
