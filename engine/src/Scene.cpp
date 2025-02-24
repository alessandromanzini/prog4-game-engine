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
	for ( auto& pObject : objects_ )
	{
		pObject->mark_for_deletion( );
	}
}

void Scene::fixed_update( )
{
	for ( auto& pObject : objects_ )
	{
		pObject->fixed_update( );
	}
}

void Scene::update( )
{
	for ( auto& pObject : objects_ )
	{
		if ( pObject->is_marked_for_deletion( ) )
		{
			deleter_.mark_element_for_deletion( *pObject );
		}
		else
		{
			pObject->update( );
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
