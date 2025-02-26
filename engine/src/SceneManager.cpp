#include "SceneManager.h"

#include "Scene.h"

#include <cassert>

using namespace engine;

void SceneManager::fixed_update( )
{
	for ( auto& scene : scenes_ )
	{
		active_scene_ptr_ = scene.get( );
		scene->fixed_update( );
	}
}

void SceneManager::update( )
{
	for ( auto& scene : scenes_ )
	{
		active_scene_ptr_ = scene.get( );
		scene->update( );
	}
}

void SceneManager::render( )
{
	for ( const auto& scene : scenes_ )
	{
		active_scene_ptr_ = scene.get( );
		scene->render( );
	}
}

void SceneManager::cleanup( )
{
	for ( auto& scene : scenes_ )
	{
		scene->cleanup( );
	}
}

Scene& SceneManager::create_scene( const std::string& name )
{
	auto& scene = scenes_.emplace_back( std::shared_ptr<Scene>( new Scene( name ) ) );
	return *scene;
}

Scene& SceneManager::get_active_scene( ) const
{
	// Active scene is not set until game loop starts.
	// If you need to delete to delete a gameobject, call the method on the scene directly.
	assert( active_scene_ptr_ != nullptr && "No active scene is set yet." );
	return *active_scene_ptr_;
}

Scene& SceneManager::get_scene( const std::string& name ) const
{
	auto scene = 
		std::find_if( scenes_.begin( ), scenes_.end( ),
		[&name]( const auto& scene ) { return scene->get_name( ) == name; } );
	assert( scene != scenes_.end( ) && "Scene not found." );
	return **scene;
}

Scene& SceneManager::get_scene( uint16_t id ) const
{
	auto scene =
		std::find_if( scenes_.begin( ), scenes_.end( ),
		[&id]( const auto& scene ) { return scene->get_id( ) == id; } );
	assert( scene != scenes_.end( ) && "Scene not found." );
	return **scene;
}
