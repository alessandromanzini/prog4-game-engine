#ifndef ENEMYAICOMPONENT_H
#define ENEMYAICOMPONENT_H

#include <framework/component/Component.h>

#include <vec2.hpp>
#include <vector>


namespace game
{
    class CharacterComponent;
    class BubbleCaptureComponent;
    class EnemyAiComponent : public engine::Component
    {
    public:
        explicit EnemyAiComponent( owner_t& owner );
        void tick( ) override;
        [[nodiscard]] engine::GameObject* get_target( ) const;

        void add_target( engine::GameObject* target );
        void set_targets( std::vector<engine::GameObject*>&& targets );

    protected:
        virtual glm::vec2 calculate_steering( glm::vec2 targetPosition ) const = 0;
        CharacterComponent* get_character_component( ) const;

    private:
        CharacterComponent* character_component_ptr_{ nullptr };
        BubbleCaptureComponent* bubble_capture_component_ptr_{ nullptr };

        std::vector<engine::GameObject*> targets_{};
        engine::GameObject* target_ptr_{ nullptr };

    };

}


#endif //!ENEMYAICOMPONENT_H
