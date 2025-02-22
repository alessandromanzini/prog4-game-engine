#include "Scene.h"
#include "GameObject.h"

#include <algorithm>

using namespace engine;

unsigned int Scene::m_idCounter = 0;

Scene::Scene( const std::string& name ) : name_( name )
{
}

void Scene::add( std::shared_ptr<GameObject> object )
{
	objects_.emplace_back( std::move( object ) );
}

void Scene::remove( std::shared_ptr<GameObject> object )
{
	object->mark_for_deletion( );
}

void Scene::remove_all( )
{
	for ( auto& object : objects_ )
	{
		object->mark_for_deletion( );
	}
}

void Scene::fixed_update( )
{
	for ( auto& object : objects_ )
	{
		object->fixed_update( );
	}
}

void Scene::update( )
{
	for ( size_t i{}; i < objects_.size( ); ++i )
	{
		if ( objects_[i]->is_marked_for_deletion( ) )
		{
			mark_index_for_deletion( i );
		}
		else
		{
			objects_[i]->update( );
		}
	}
}

void Scene::render( ) const
{
	for ( const auto& object : objects_ )
	{
		object->render( );
	}
}

void Scene::cleanup( )
{
	if ( is_cleanup_needed( ) )
	{
		// Remove elements in one pass using erase-remove idiom
		// 'i' is the index of the object in the vector
		// 'i' gets checked against the indices_to_destroy_ set, returning 0 for no match or 1 for match
		std::erase_if( objects_,
			[this, i = size_t( 0 )]( auto ) mutable { return indices_to_destroy_.count( i++ ); } );
		indices_to_destroy_.clear( );
	}
}

bool Scene::is_cleanup_needed( ) const noexcept
{
	return !indices_to_destroy_.empty( );
}

void Scene::mark_index_for_deletion( size_t index )
{
	indices_to_destroy_.insert( index );
}
