#ifndef OBJECTINITIALIZERS_H
#define OBJECTINITIALIZERS_H

#include <framework/GameObject.h>
#include <framework/resource/Font.h>

#include <memory>


namespace game
{
    class ScoreComponent;
    class CharacterController;

    void create_join_menu( engine::GameObject& join );

    void create_bub( engine::GameObject& object, ScoreComponent* score );
    void create_bob( engine::GameObject& object, ScoreComponent* score );
    void create_zenchan( engine::GameObject& object, glm::vec2 position, std::vector<engine::GameObject*> targets = {} );
    void create_maita( engine::GameObject& object, glm::vec2 position, std::vector<engine::GameObject*> targets = {} );

    void create_fruit( engine::GameObject& object, const std::string& texturePath, int value, glm::vec2 position, glm::vec2 spawnerPosition );

    void create_grid( engine::GameObject& object );
    void create_score( engine::GameObject& object, std::shared_ptr<engine::Font> font );

    void create_arcade( engine::Scene& scene, const std::vector<CharacterController*>& controllers, const std::shared_ptr<engine::Font>& font );
    void create_versus( engine::Scene& scene, const std::vector<CharacterController*>& controllers, const std::shared_ptr<engine::Font>& font );

}


#endif //!OBJECTINITIALIZERS_H
