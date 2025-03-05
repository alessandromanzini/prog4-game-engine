#include "singletons/ScenePool.h"

// +--------------------------------+
// | Project Headers				|
// +--------------------------------+
#include "Scene.h"

// +--------------------------------+
// | Standard Headers				|
// +--------------------------------+
#include <cassert>

using namespace engine;

ScenePool& engine::SCENE_POOL = ScenePool::get_instance( );

void ScenePool::fixed_update( )
{
	for ( auto& scene : scenes_ )
	{
		active_scene_ptr_ = scene.get( );
		scene->fixed_update( );
	}
}

void ScenePool::update( )
{
	for ( auto& scene : scenes_ )
	{
		active_scene_ptr_ = scene.get( );
		scene->update( );
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

void ScenePool::cleanup( )
{
	for ( auto& scene : scenes_ )
	{
		scene->cleanup( );
	}
}

Scene& ScenePool::create_scene( const std::string& name )
{
	auto& scene = scenes_.emplace_back( std::make_unique<Scene>( name ) );
	return *scene;
}

Scene& ScenePool::get_active_scene( ) const
{
	// Active scene is not set until game loop starts.
	// If you need to delete to delete a gameobject, call the method on the scene directly.
	assert( active_scene_ptr_ != nullptr && "No active scene is set yet." );
	return *active_scene_ptr_;
}

Scene& ScenePool::get_scene( const std::string& name ) const
{
	auto scene = 
		std::find_if( scenes_.begin( ), scenes_.end( ),
		[&name]( const auto& scene ) { return scene->get_name( ) == name; } );
	assert( scene != scenes_.end( ) && "Scene not found." );
	return **scene;
}

Scene& ScenePool::get_scene( uint16_t id ) const
{
	auto scene =
		std::find_if( scenes_.begin( ), scenes_.end( ),
		[&id]( const auto& scene ) { return scene->get_id( ) == id; } );
	assert( scene != scenes_.end( ) && "Scene not found." );
	return **scene;
}

void ScenePool::render_ui( )
{
	// TODO: add components for UI
}
