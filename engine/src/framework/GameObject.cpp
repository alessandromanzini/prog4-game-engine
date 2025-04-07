#include <GameObject.h>

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <components/Component.h>
#include <framework/GameObjectView.h>
#include <singletons/Renderer.h>
#include <singletons/ResourceManager.h>


namespace engine
{
    // GameObject::GameObject( )
    //     : view_ptr_( new GameObjectView{ *this } ) { }


    GameObject::~GameObject( ) noexcept
    {
        if ( parent_ptr_ != nullptr )
        {
            parent_ptr_->remove_child( this );
        }
        for ( size_t i{}; i < children_.size( ); ++i )
        {
            children_[i]->set_parent( nullptr, false );
        }
    }


    void GameObject::fixed_update( )
    {
        for ( const auto& [key, component] : components_ )
        {
            component->fixed_update( );
        }
    }


    void GameObject::update( )
    {
        for ( const auto& [key, component] : components_ )
        {
            component->update( );
        }
    }


    void GameObject::render( ) const
    {
        for ( const auto& [key, component] : components_ )
        {
            component->render( );
        }
    }


    void GameObject::cleanup( )
    {
        if ( deleter_.is_cleanup_needed( ) )
        {
            deleter_.cleanup( components_ );
        }
    }


    void GameObject::set_parent( GameObject* const parent, const bool keepWorldPosition /* = true */ )
    {
        // Parent validation
        if ( is_child( parent ) || parent == this || parent == parent_ptr_ )
        {
            return;
        }

        if ( parent == nullptr )
        {
            // If becoming root object, local transform becomes world space transform
            set_local_transform( get_world_transform( ) );
        }
        else
        {
            // If parent is provided, we need to adjust the local transform to keep the world position
            // and set it dirty to recalculate the world transform.
            if ( keepWorldPosition )
            {
                // TODO: test if rotation is canceled
                const auto translation = get_world_transform( ).get_position( ) - parent->get_world_transform( ).get_position( );
                set_local_transform(Transform::from_translation( translation ) );
            }
            set_transform_dirty( );
        }

        // Re-parenting logic
        if ( parent_ptr_ != nullptr ) parent_ptr_->remove_child( this );
        if ( parent != nullptr ) parent->add_child( this );
        parent_ptr_ = parent;
    }


    const Transform& GameObject::get_world_transform( )
    {
        update_world_transform( );
        return world_transform_;
    }


    const Transform& GameObject::get_local_transform( ) const
    {
        return local_transform_;
    }


    void GameObject::set_world_transform( const Transform& transform )
    {
        if ( parent_ptr_ == nullptr )
        {
            set_local_transform( transform );
        }
        else
        {
            const glm::mat3x3 parentInverse{ glm::inverse( parent_ptr_->get_world_transform( ).get_matrix( ) ) };
            set_local_transform( parentInverse * transform.get_matrix( ) );
        }
    }


    void GameObject::set_local_transform( const Transform& transform )
    {
        local_transform_ = transform;
        set_transform_dirty( );
    }


    void GameObject::set_local_transform( Transform&& transform )
    {
        local_transform_ = std::move( transform );
        set_transform_dirty( );
    }


    void GameObject::remove_component( Component& component )
    {
        deleter_.mark_element_for_deletion( &component );
    }


    std::vector<GameObject*>& GameObject::get_children( )
    {
        return children_;
    }


    const std::vector<GameObject*>& GameObject::get_children( ) const
    {
        return children_;
    }


    void GameObject::collect_children( std::vector<GameObject*>& children ) const
    {
        for ( auto* child : children_ )
        {
            children.push_back( child );
            child->collect_children( children );
        }
    }


    bool GameObject::is_child( const GameObject* const gameObject ) const
    {
        return std::ranges::any_of( children_,
                                    [gameObject]( const auto* child )
                                        {
                                            return child == gameObject || child->is_child( gameObject );
                                        } );
    }


    void GameObject::add_child( GameObject* gameObject )
    {
        children_.push_back( gameObject );
    }


    void GameObject::remove_child( GameObject* gameObject )
    {
        std::erase( children_, gameObject );
    }


    bool GameObject::has_children( ) const
    {
        return !children_.empty( );
    }


    void GameObject::set_transform_dirty( )
    {
        transform_dirty_ = true;

        // TODO: optimize this
        for ( const auto child : children_ )
        {
            child->set_transform_dirty( );
        }
    }


    void GameObject::update_world_transform( )
    {
        if ( transform_dirty_ )
        {
            if ( parent_ptr_ == nullptr )
            {
                world_transform_ = get_local_transform( );
            }
            else
            {
                world_transform_ = parent_ptr_->get_world_transform( ) * local_transform_;
            }
            transform_dirty_ = false;
        }
    }

}
