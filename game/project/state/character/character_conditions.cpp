#include "character_conditions.h"

#include <framework/component/physics/PhysicsComponent.h>
#include <framework/resource/texture/Sprite2D.h>


using namespace engine;

namespace game::condition
{
    constexpr float IDLE_EPSILON{ .1f };
    constexpr float GROUNDED_EPSILON{ 1.f };


    glm::vec2 get_relative_movement( const Blackboard& blackboard )
    {
        glm::vec2 relative{};
        if ( not blackboard.retrieve( UID( "relative_movement" ), relative ) )
        {
            printf( "Error: No relative movement found in the blackboard.\n" );
            return {};
        }
        return relative;
    }


    // +----------------------------------+
    // | IS ANIMATION COMPLETED CONDITION |
    // +----------------------------------+
    bool IsAnimationCompletedCondition::evaluate( Blackboard& blackboard ) const
    {
        Sprite2D* sprite{ nullptr };
        blackboard.retrieve( UID( "current_sprite" ), sprite );
        return sprite && sprite->is_animation_completed( );
    }


    // +---------------------------+
    // | IS RISING CONDITION       |
    // +---------------------------+
    bool IsRisingCondition::evaluate( Blackboard& blackboard ) const
    {
        const glm::vec2 relative = get_relative_movement( blackboard );
        return relative.y < -FLT_EPSILON;
    }


    // +----------------------------------+
    // | IS FALLING CONDITION             |
    // +----------------------------------+
    bool IsFallingCondition::evaluate( Blackboard& blackboard ) const
    {
        const glm::vec2 relative = get_relative_movement( blackboard );
        return relative.y > FLT_EPSILON;
    }


    // +---------------------------+
    // | IS GROUNDED CONDITION     |
    // +---------------------------+
    bool IsGroundedCondition::evaluate( Blackboard& blackboard ) const
    {
        const glm::vec2 relative = get_relative_movement( blackboard );
        return relative.y * relative.y < GROUNDED_EPSILON * GROUNDED_EPSILON;
    }


    // +----------------------------------+
    // | IS MOVING HORZ CONDITION         |
    // +----------------------------------+
    bool IsMovingHorzCondition::evaluate( Blackboard& blackboard ) const
    {
        const glm::vec2 relative = get_relative_movement( blackboard );
        return relative.x * relative.x > IDLE_EPSILON * IDLE_EPSILON;
    }

}
