#ifndef OBJECTINITIALIZERS_H
#define OBJECTINITIALIZERS_H

#include <framework/GameObject.h>
#include <framework/resource/Font.h>

#include <memory>


namespace game
{
    class ScoreComponent;

    void create_bub( engine::GameObject& object, ScoreComponent* score );
    void create_bob( engine::GameObject& object, ScoreComponent* score );
    void create_zenchan( engine::GameObject& object, glm::vec2 position, std::vector<engine::GameObject*> targets = {} );
    void create_maita( engine::GameObject& object, glm::vec2 position, std::vector<engine::GameObject*> targets = {} );

    void create_fruit( engine::GameObject& object, const std::string& texturePath, int value, glm::vec2 position = { 0.f, 0.f } );

    void create_grid( engine::GameObject& object );
    void create_score( engine::GameObject& object, std::shared_ptr<engine::Font> font );

}


#endif //!OBJECTINITIALIZERS_H
