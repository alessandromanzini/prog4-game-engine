#include <framework/component/physics/ColliderComponent.h>
#include <singleton/GameInstance.h>


namespace engine
{
    ColliderComponent::ColliderComponent( owner_t& owner )
        : Component{ owner }
    {
        s_colliders_.emplace_back( this );
    }


    ColliderComponent::~ColliderComponent( )
    {
        std::erase( s_colliders_, this );
    }


    void ColliderComponent::tick( )
    {
        // Check for overlaps with other colliders
        for ( auto* const collider : overlapping_colliders_.data( ) )
        {
            auto& other{ *collider };
            auto pair = make_cross_check_pair( other );
            if ( s_colliders_cross_check_.contains( pair ) )
            {
                continue;
            }
            if ( not hit_test( other, true ) )
            {
                handle_end_overlap( other );
                other.handle_end_overlap( *this );
                s_colliders_cross_check_.insert( std::move( pair ) );
            }
        }

        // Check for overlaps with colliders that are not in the overlapping set
        for ( auto* const collider : s_colliders_ )
        {
            if ( collider == this )
            {
                continue;
            }

            auto& other{ *collider };
            auto pair = make_cross_check_pair( other );
            if ( s_colliders_cross_check_.contains( pair ) )
            {
                continue;
            }
            hit_test( *collider );
            s_colliders_cross_check_.insert( std::move( pair ) );
        }
    }


    void ColliderComponent::late_tick( )
    {
        // TODO: This should be done in a better way
        s_colliders_cross_check_.clear( );
    }


    bool ColliderComponent::hit_test( ColliderComponent& other, const bool ignoreOverlaps )
    {
        if ( not ignoreOverlaps && overlapping_colliders_.contains( &other ) )
        {
            return true;
        }
        if ( hit_test_impl( other ) )
        {
            if ( not ignoreOverlaps )
            {
                handle_begin_overlap( other );
                other.handle_begin_overlap( *this );
            }
            return true;
        }
        return false;
    }


    void ColliderComponent::offset_by( const glm::vec2& offset )
    {
        offset_ += offset;
    }


    void ColliderComponent::offset_to( const glm::vec2& offset )
    {
        offset_ = offset;
    }


    glm::vec2 ColliderComponent::get_offset( ) const
    {
        return offset_;
    }


    bool ColliderComponent::hit_test_impl( const ColliderComponent& other ) const
    {
        const auto offset = offset_ + get_owner( ).get_world_transform( ).get_position( );
        for ( auto pivot : get_pivots( ) )
        {
            if ( other.hit_test( pivot + offset ) )
            {
                return true;
            }
        }
        return false;
    }


    void ColliderComponent::handle_begin_overlap( ColliderComponent& other )
    {
        on_begin_overlap_dispatcher_.broadcast( *this, other );
        overlapping_colliders_.insert( &other );
    }


    void ColliderComponent::handle_end_overlap( ColliderComponent& other )
    {
        on_end_overlap_dispatcher_.broadcast( *this, other );
        overlapping_colliders_.remove( &other );
    }

    std::pair<ColliderComponent*, ColliderComponent*> ColliderComponent::make_cross_check_pair( ColliderComponent& other )
    {
        if ( this < &other )
        {
            return std::make_pair( this, &other );
        }
        return std::make_pair( &other, this );
    }

}
