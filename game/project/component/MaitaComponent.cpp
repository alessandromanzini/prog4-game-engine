#include "MaitaComponent.h"

#include <framework/GameObject.h>
#include <singleton/GameTime.h>

#include "CharacterComponent.h"


namespace game
{
    MaitaComponent::MaitaComponent( owner_t& owner )
        : EnemyAiComponent{ owner } { }


    glm::vec2 MaitaComponent::calculate_steering( const glm::vec2 targetPosition )
    {
        const auto [success, steering, difference] = process_default_path_finding( targetPosition );
        if ( success )
        {
            steering_ = steering;
        }
        // If the target is in range, attack
        if ( abs( difference.x ) < SHOOT_RANGE_.x && abs( difference.y ) < SHOOT_RANGE_.y )
        {
            get_character_component( )->attack( );
        }
        return steering_;
    }

}
