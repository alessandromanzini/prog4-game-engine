#include <components/RotatingComponent.h>

#include <framework/Transform.h>
#include <singletons/GameTime.h>


using namespace engine;

namespace game
{
    RotatingComponent::RotatingComponent( owner_t& owner, const float rotationSpeed )
        : Component( owner )
        , rotation_speed_{ rotationSpeed } { }


    void RotatingComponent::update( )
    {
        const TransformOperator op{ get_owner( ) };
        op.rotate( rotation_speed_ * GAME_TIME.get_delta_time( ) );
    }

}
