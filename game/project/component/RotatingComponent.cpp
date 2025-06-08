#include <component/RotatingComponent.h>

#include <../../../engine/include/public/framework/Transform.h>
#include <../../../engine/include/public/singleton/GameTime.h>


using namespace engine;

namespace game
{
    RotatingComponent::RotatingComponent( owner_t& owner, const float rotationSpeed )
        : Component( owner )
        , rotation_speed_{ rotationSpeed } { }


    void RotatingComponent::tick( )
    {
        const TransformOperator op{ get_owner( ) };
        op.rotate( rotation_speed_ * GAME_TIME.get_delta_time( ) );
    }

}
