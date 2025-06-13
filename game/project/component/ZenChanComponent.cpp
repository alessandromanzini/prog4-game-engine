#include "ZenChanComponent.h"

#include <framework/GameObject.h>
#include <singleton/GameTime.h>

#include "CharacterComponent.h"


namespace game
{
    ZenChanComponent::ZenChanComponent( owner_t& owner )
        : EnemyAiComponent{ owner } { }


    glm::vec2 ZenChanComponent::calculate_steering( const glm::vec2 targetPosition )
    {
        if ( const auto [success, steering, difference] = process_default_path_finding( targetPosition ); success )
        {
            steering_ = steering;
        }
        return steering_;
    }

}
