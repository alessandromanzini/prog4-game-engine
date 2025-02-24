#include "GameObjectView.h"

// +--------------------------------+
// | Project Headers				|
// +--------------------------------+

// +--------------------------------+
// | Standard Headers				|
// +--------------------------------+
#include <stdexcept>

using namespace engine;

const engine::Transform& GameObjectView::get_world_transform( )
{
	return gameobject_ptr_->get_world_transform( );
}

const engine::Transform& GameObjectView::get_local_transform( ) const
{
	return gameobject_ptr_->get_local_transform( );
}

void GameObjectView::set_world_transform( const Transform& transform )
{
	gameobject_ptr_->set_world_transform( transform );
}

void GameObjectView::set_world_transform( Transform&& transform )
{
	gameobject_ptr_->set_world_transform( std::move( transform ) );
}

void GameObjectView::set_local_transform( const Transform& transform )
{
	gameobject_ptr_->set_local_transform( transform );
}

void GameObjectView::set_local_transform( Transform&& transform )
{
	gameobject_ptr_->set_local_transform( std::move( transform ) );
}

// PRIVATE CONSTRUCTOR
GameObjectView::GameObjectView( GameObject& gameobject )
	: gameobject_ptr_{ &gameobject }
{
}
