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
		// Can optimize by calling mark_index_for_deletion from the game object
		if ( objects_[i]->is_marked_for_deletion( ) )
		{
			deleter_.mark_index_for_deletion( i );
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
	if ( deleter_.is_cleanup_needed( ) )
	{
		deleter_.cleanup( objects_ );
	}

	for ( auto& object : objects_ )
	{
		object->cleanup( );
	}
}
