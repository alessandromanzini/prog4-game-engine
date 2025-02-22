#include "GameObject.h"

// +--------------------------------+
// | Standard Headers				|
// +--------------------------------+
#include <string>

// +--------------------------------+
// | Project Headers				|
// +--------------------------------+
#include "BaseComponent.h"
#include "GameObjectView.h"
#include "ResourceManager.h"
#include "Renderer.h"

using namespace engine;

void GameObject::fixed_update( )
{
	for ( const auto& [key, component] : components_ )
	{
		component->fixed_update( );
	}
}

void GameObject::update()
{
	for ( const auto& [key, component] : components_ )
	{
		component->update( );
	}
}

void GameObject::render() const
{
	for ( const auto& [key, component] : components_ )
	{
		component->render( );
	}
}

Transform& GameObject::get_transform( )
{
	return transform_;
}
