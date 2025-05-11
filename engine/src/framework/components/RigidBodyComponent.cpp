#include <framework/GameObject.h>
#include <framework/components/RigidBodyComponent.h>

#include <framework/Transform.h>
#include <singletons/GameTime.h>


namespace engine
{
    RigidBodyComponent::RigidBodyComponent( owner_t& owner )
        : Component{ owner } { }


    glm::vec2 RigidBodyComponent::get_velocity( ) const
    {
        return velocity_;
    }


    void RigidBodyComponent::add_velocity( const glm::vec2 velocity )
    {
        velocity_ += velocity;

        if ( velocity_.x > 0.f )
        {
            face_direction_ = FaceDirection::RIGHT;
        }
        else if ( velocity_.x < 0.f )
        {
            face_direction_ = FaceDirection::LEFT;
        }
    }


    void RigidBodyComponent::reset_horizontal_velocity( )
    {
        velocity_ = { 0.f, velocity_.y };
    }


    void RigidBodyComponent::tick( )
    {
        if ( velocity_.y != 0.f )
        {
            velocity_ += glm::vec2{ 0.f, 125.f * GAME_TIME.get_delta_time( ) };
        }
        const auto displacement{ velocity_ * GAME_TIME.get_delta_time( ) };

        // TEMP GRAVITY CODE
        if ( const auto pos = get_owner(  ).get_world_transform(  ).get_position(  );
            pos.y + displacement.y > 375.f )
        {
            get_owner(  ).set_world_transform( Transform::from_translation( { pos.x + displacement.x, 375.f } ) );
            velocity_.y = 0.f;
        }
        else
        {
            TransformOperator{ get_owner( ) }
                .translate( displacement );
        }

    }


    FaceDirection RigidBodyComponent::get_face_direction( ) const
    {
        return face_direction_;
    }

}
