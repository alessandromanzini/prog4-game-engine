#include "GameObject.h"

// +--------------------------------+
// | Standard Headers				|
// +--------------------------------+
#include <string>

// +--------------------------------+
// | Project Headers				|
// +--------------------------------+
#include "BaseComponent.h"
#include "GameObjectView.h"
#include "ResourceManager.h"
#include "Renderer.h"

using namespace engine;

GameObject::GameObject( )
	: view_ptr_( new GameObjectView{ *this } )
{
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
		// Can optimize by calling mark_index_for_deletion from the component
		if ( component->is_marked_for_deletion( ) )
		{
			deleter_.mark_index_for_deletion( key );
		}
		else
		{
			component->update( );
		}
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
	deleter_.cleanup( components_ );
}

void GameObject::mark_for_deletion( )
{
	Deletable::mark_for_deletion( );
	for ( auto pChild : children_ )
	{
		pChild->mark_for_deletion( );
	}
}

void GameObject::set_parent( GameObject* const pParent, bool keepWorldPosition /* = true */ )
{
	// Parent validation
	if ( is_child( pParent ) || pParent == this || pParent == parent_ptr_ )
	{
		return;
	}

	// Remove self from parent's children if not root object
	if ( parent_ptr_ != nullptr )
	{
		parent_ptr_->remove_child( this );
	}

	// Set new parent
	// If new parent is nullptr, self becomes root object
	// else, add self to new parent's children
	if ( pParent == nullptr )
	{
		set_local_transform( get_world_transform( ) );
	}
	else
	{
		if ( keepWorldPosition )
		{
			set_local_transform( get_world_transform( ).get_position() - pParent->get_world_transform( ).get_position() );
		}
		set_transform_dirty( );

		pParent->add_child( this );
	}
	parent_ptr_ = pParent;
}

const Transform& GameObject::get_world_transform( )
{
	update_world_transform( );
	return world_transform_;
}

const engine::Transform& GameObject::get_local_transform( ) const
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
		glm::mat3x3 parentInverse{ glm::inverse( parent_ptr_->get_world_transform( ).get_matrix( ) ) };
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

bool GameObject::is_child( GameObject* const pGameObject ) const
{
	for ( auto pChild : children_ )
	{
		if ( pChild == pGameObject )
		{
			return true;
		}
	}
	return false;
}

void GameObject::add_child( GameObject* pGameObject )
{
	children_.push_back( pGameObject );
}

void GameObject::remove_child( GameObject* pGameObject )
{
	children_.erase( std::remove( children_.begin( ), children_.end( ), pGameObject ), children_.end( ) );
}

void GameObject::update_world_transform( )
{
	if ( transform_dirty_ )
	{
		if ( parent_ptr_ == nullptr )
		{
			set_world_transform( get_local_transform( ) );
		}
		else
		{
			world_transform_ = parent_ptr_->get_world_transform( ) + local_transform_;
		}
		transform_dirty_ = false;
	}
}
