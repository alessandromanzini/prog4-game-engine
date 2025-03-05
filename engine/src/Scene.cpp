#include "Scene.h"

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include "GameObject.h"

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <algorithm>

namespace engine
{
	uint16_t Scene::s_id_counter_ = 0;

	void Scene::add( std::unique_ptr<GameObject> pObject )
	{
		objects_.emplace_back( std::move( pObject ) );
	}

	GameObject* Scene::create_object( )
	{
		return objects_.emplace_back( std::make_unique<GameObject>( ) ).get( );
	}

	const std::string& Scene::get_name( ) const
	{
		return name_;
	}

	uint16_t Scene::get_id( ) const
	{
		return id_;
	}

	void Scene::remove( GameObject* pObject )
	{
		if ( pObject != nullptr )
		{
			deleter_.mark_element_for_deletion( pObject );

			// Remove children as well
			for ( auto pChild : pObject->get_children( ) )
			{
				remove( pChild );
			}
		}
	}

	void Scene::remove_all( )
	{
		for ( auto& pObject : objects_ )
		{
			deleter_.mark_element_for_deletion( pObject.get( ) );
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
			pObject->update( );
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
		// Delete marked objects
		if ( deleter_.is_cleanup_needed( ) )
		{
			deleter_.cleanup( objects_ );
		}

		// Cleanup objects
		for ( auto& object : objects_ )
		{
			object->cleanup( );
		}
	}

	Scene::Scene( const std::string& name )
		: name_( name )
		, id_( s_id_counter_++ )
	{
	}
}
