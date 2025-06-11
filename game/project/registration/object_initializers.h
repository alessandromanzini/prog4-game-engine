#ifndef OBJECTINITIALIZERS_H
#define OBJECTINITIALIZERS_H

#include <framework/GameObject.h>


namespace game
{
    void create_bub( engine::GameObject& object, glm::vec2 position );
    void create_zenchan( engine::GameObject& object, glm::vec2 position, std::vector<engine::GameObject*> targets = {} );

    void create_fruit( engine::GameObject& object, const std::string& texturePath, int value, glm::vec2 position = { 0.f, 0.f } );

    void create_grid( engine::GameObject& object );

}


#endif //!OBJECTINITIALIZERS_H
