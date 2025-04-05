#include <components/RotatingComponent.h>

#include <singletons/GameTime.h>


using namespace engine;

namespace game
{
    RotatingComponent::RotatingComponent( GameObjectView& owner, const float rotationSpeed )
        : Component( owner )
        , rotation_speed_{ rotationSpeed } { }


    void RotatingComponent::update( )
    {
        const auto rotation{ rotation_speed_ * GAME_TIME.get_delta_time( ) };
        auto transform =
                glm::rotate( glm::mat3x3{ 1.0 }, rotation ) * get_owner( ).get_local_transform( ).get_matrix( );
        get_owner( ).set_local_transform( transform );
    }

}
