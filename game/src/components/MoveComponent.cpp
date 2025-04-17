#include <components/MoveComponent.h>
#include <framework/Transform.h>

#include <singletons/GameTime.h>


namespace game
{
    MoveComponent::MoveComponent( owner_t& owner, const float speed )
        : Component{ owner }
        , speed_{ speed } { }


    void MoveComponent::move( const glm::vec2 direction ) const
    {
        engine::TransformOperator{ get_owner( ) }
                .translate( normalize( direction ) * speed_ * engine::GAME_TIME.get_delta_time( ) );
    }

}
