#include "ZenChanComponent.h"

#include <framework/GameObject.h>

#include "CharacterComponent.h"


namespace game
{
    ZenChanComponent::ZenChanComponent( owner_t& owner )
        : EnemyAiComponent{ owner }
    {
    }


    glm::vec2 ZenChanComponent::calculate_steering( const glm::vec2 targetPosition ) const
    {
        auto direction = targetPosition - get_owner( ).get_world_transform( ).get_position(  );
        if ( direction.y < -50.f )
        {
            get_character_component(  )->jump(  );
        }
        return { direction.x, 0.f };
    }

}
