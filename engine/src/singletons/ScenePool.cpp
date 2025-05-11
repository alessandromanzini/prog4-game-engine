#include <singletons/ScenePool.h>

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <framework/Scene.h>

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <cassert>


namespace engine
{
    ScenePool& SCENE_POOL = ScenePool::get_instance( );


    void ScenePool::fixed_tick( )
    {
        for ( auto& scene : scenes_ )
        {
            active_scene_ptr_ = scene.get( );
            scene->fixed_tick( );
        }
    }


    void ScenePool::tick( )
    {
        for ( auto& scene : scenes_ )
        {
            active_scene_ptr_ = scene.get( );
            scene->tick( );
        }
    }


    void ScenePool::render( )
    {
        for ( const auto& scene : scenes_ )
        {
            active_scene_ptr_ = scene.get( );
            scene->render( );
        }
    }


    void ScenePool::cleanup( ) const
    {
        for ( const auto& scene : scenes_ )
        {
            scene->cleanup( );
        }
    }


    Scene& ScenePool::create_scene( const std::string& name )
    {
        const auto& scene = scenes_.emplace_back( std::make_unique<Scene>( name ) );
        return *scene;
    }


    void ScenePool::unload_all_scenes( )
    {
        scenes_.clear( );
    }


    Scene& ScenePool::get_active_scene( ) const
    {
        // Active scene is not set until game loop starts.
        // If you need to delete a gameobject, call the method on the scene directly.
        assert( active_scene_ptr_ != nullptr && "No active scene is set yet." );

        // ReSharper disable once CppDFANullDereference
        return *active_scene_ptr_;
    }


    Scene& ScenePool::get_scene( const std::string& name ) const
    {
        const auto scene =
                std::ranges::find_if( scenes_,
                                      [&name]( const auto& scene ) { return scene->get_name( ) == name; } );
        assert( scene != scenes_.end( ) && "Scene not found." );
        return **scene;
    }


    Scene& ScenePool::get_scene( uint16_t id ) const
    {
        const auto scene =
                std::ranges::find_if( scenes_,
                                      [&id]( const auto& scene ) { return scene->get_id( ) == id; } );
        assert( scene != scenes_.end( ) && "Scene not found." );
        return **scene;
    }

}
