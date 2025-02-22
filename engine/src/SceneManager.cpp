#include "SceneManager.h"

#include "Scene.h"

using namespace engine;

void SceneManager::fixed_update( )
{
	for ( auto& scene : scenes_ )
	{
		scene->fixed_update( );
	}
}

void SceneManager::update()
{
	for(auto& scene : scenes_)
	{
		scene->update();
	}
}

void SceneManager::render()
{
	for (const auto& scene : scenes_)
	{
		scene->render();
	}
}

void SceneManager::cleanup( )
{
	for ( auto& scene : scenes_ )
	{
		scene->cleanup( );
	}
}

Scene& SceneManager::create_scene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	scenes_.push_back(scene);
	return *scene;
}
