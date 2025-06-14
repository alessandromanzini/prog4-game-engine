#include "FruitComponent.h"

#include <framework/GameObject.h>
#include <singleton/GameTime.h>

#include <algorithm>
#include <random>


namespace game
{
    float random_float( const float min, const float max )
    {
        static std::random_device rd;     // Seed
        static std::mt19937 gen( rd( ) ); // Mersenne Twister RNG
        std::uniform_real_distribution dist( min, max );
        return dist( gen );
    }


    glm::vec2 random_vec( const float min = 0.0f, const float max = 1.0f )
    {
        return glm::vec2( random_float( min, max ), random_float( min, max ) );
    }


    FruitComponent::FruitComponent( owner_t& owner, const int value, const int bounces, const glm::vec2 preferredDirection )
        : Component( owner )
        , value_{ value }
        , bounces_( bounces )
        , direction_{ normalize( preferredDirection + random_vec( 0.f, .25f ) ) } { }


    void FruitComponent::tick( )
    {
        iframes_ += engine::GAME_TIME.get_delta_time( );
        if ( is_moving( ) )
        {
            get_owner( ).get_transform_operator( ).translate( direction_ * SPEED_ * engine::GAME_TIME.get_delta_time( ) );
        }
    }


    void FruitComponent::bounce( const glm::vec2 normal, const float depth )
    {
        --bounces_;
        direction_ = reflect( direction_, normal );
        if ( dot( normal, glm::vec2{ 0.f, -1.f } ) < .9f )
        {
            bounces_ = std::max( 1, bounces_ );
        }
        if ( not is_moving( ) )
        {
            get_owner( ).get_transform_operator( ).translate( normal * ( depth + .1f ) );
        }
    }


    int FruitComponent::get_fruit_value( ) const
    {
        return value_;
    }


    bool FruitComponent::is_capturable( ) const
    {
        return iframes_ > IFRAMES_DURATION_;
    }


    bool FruitComponent::is_moving( ) const
    {
        return bounces_ > 0;
    }

}
