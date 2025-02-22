#include "GameObjectView.h"

// +--------------------------------+
// | Project Headers				|
// +--------------------------------+

// +--------------------------------+
// | Standard Headers				|
// +--------------------------------+
#include <stdexcept>

using namespace engine;

Transform& GameObjectView::get_transform( ) const
{
	return gameobject_ptr_->get_transform( );
}

// PRIVATE CONSTRUCTOR
GameObjectView::GameObjectView( GameObject& gameobject )
	: gameobject_ptr_{ &gameobject }
{
}
