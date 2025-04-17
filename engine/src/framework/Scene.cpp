#include <framework/Scene.h>

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <framework/GameObject.h>

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <algorithm>
#include <utility>


namespace engine
{
    uint16_t Scene::s_id_counter_ = 0;


    void Scene::add( std::unique_ptr<GameObject> object )
    {
        objects_.emplace_back( std::move( object ) );
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


    void Scene::remove( GameObject* object )
    {
        if ( object != nullptr )
        {
            deleter_.mark_element_for_deletion( object );

            // Remove children as well
            for ( auto* child : object->get_children( ) )
            {
                remove( child );
            }
        }
    }


    void Scene::remove_all( )
    {
        for ( auto& object : objects_ )
        {
            deleter_.mark_element_for_deletion( object.get( ) );
        }
    }


    void Scene::fixed_update( ) const
    {
        for ( const auto& object : objects_ )
        {
            object->fixed_update( );
        }
    }


    void Scene::update( ) const
    {
        for ( const auto& object : objects_ )
        {
            object->update( );
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
        for ( const auto& object : objects_ )
        {
            object->cleanup( );
        }
    }


    Scene::Scene( std::string name )
        : name_( std::move( name ) )
        , id_( s_id_counter_++ ) { }

}
