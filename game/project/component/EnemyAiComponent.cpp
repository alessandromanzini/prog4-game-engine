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
        // If the bubble capture component is captured, do not move
        if ( bubble_capture_component_ptr_->is_captured( ) )
        {
            return;
        }

        // Find the closest target
        target_ptr_ = nullptr;
        const auto selfPosition = get_owner( ).get_world_transform( ).get_position( );
        float closestDistance{ std::numeric_limits<float>::max( ) };
        for ( const auto& target : targets_ )
        {
            if ( auto character = target->get_component<CharacterComponent>( );
                character && character.value( ).is_iframing( ).first )
            {
                continue;
            }

            auto targetPosition = target->get_world_transform( ).get_position( );
            if ( const auto distance = dot( targetPosition - selfPosition, targetPosition - selfPosition );
                distance < closestDistance )
            {
                closestDistance = distance;
                target_ptr_     = target;
            }
        }

        // If no target is found, do not move
        if ( not target_ptr_ )
        {
            character_component_ptr_->move( {} );
        }
        // Calculate the steering vector towards the target
        else if ( const auto steering = calculate_steering( target_ptr_->get_world_transform( ).get_position( ) );
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


    PathFindingResult EnemyAiComponent::process_default_path_finding( const glm::vec2 targetPosition ) const
    {
        const glm::vec2 difference = targetPosition - get_owner( ).get_world_transform( ).get_position( );

        // Target is above the self
        if ( difference.y < -DEFAULT_COMMIT_RANGE_.y )
        {
            // Jump to reach the target
            get_character_component( )->jump( );
        }
        // Target is below the self
        else if ( difference.y > DEFAULT_COMMIT_RANGE_.y )
        {
            // If moving downwards, recalculate the steering, else commit
            if ( const glm::vec2 relative = get_character_component( )->get_relative_movement( );
                not get_character_component( )->is_falling( ) && not( abs( relative.x ) < 1.f ) )
            {
                return { false, { 0.f, 0.f }, difference };
            }
        }
        return { true, { difference.x, 0.f }, difference };
    }

}
