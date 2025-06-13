#ifndef ENEMYAICOMPONENT_H
#define ENEMYAICOMPONENT_H

#include <framework/component/Component.h>

#include <vec2.hpp>
#include <vector>


namespace game
{
    struct PathFindingResult
    {
        bool success{ false };
        glm::vec2 steering{ 0.f };
        glm::vec2 difference{ 0.f };
    };


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
        virtual glm::vec2 calculate_steering( glm::vec2 targetPosition ) = 0;
        CharacterComponent* get_character_component( ) const;

        PathFindingResult process_default_path_finding( glm::vec2 targetPosition ) const;

    private:
        static constexpr glm::vec2 DEFAULT_COMMIT_RANGE_{ 1000.f, 60.f };

        CharacterComponent* character_component_ptr_{ nullptr };
        BubbleCaptureComponent* bubble_capture_component_ptr_{ nullptr };

        std::vector<engine::GameObject*> targets_{};
        engine::GameObject* target_ptr_{ nullptr };

    };

}


#endif //!ENEMYAICOMPONENT_H
