#ifndef LEVELLOADERCOMPONENT_H
#define LEVELLOADERCOMPONENT_H

#include <vec2.hpp>
#include <framework/component/Component.h>

#include <vector>
#include <framework/GameObject.h>


namespace game
{
    class LevelLoaderComponent final : public engine::Component
    {
    public:
        explicit LevelLoaderComponent( owner_t& owner, std::vector<engine::GameObject*> players, std::vector<const char*> levelPaths, bool randomRound, bool spawnEnemies = true );
        void tick( ) override;
        void destroy_all_enemies( );

    private:
        static constexpr float LEVEL_TRANSITION_SPEED_{ 200.f };
        static constexpr float LEVEL_PERSISTANCE_SECONDS_{ 5.f };

        const std::vector<const char*> level_paths_{};
        std::vector<engine::GameObject*> players_{};

        uint32_t current_level_{ 0 };
        uint32_t enemy_count_{ 0 };
        engine::GameObject* current_level_object_ptr_{ nullptr };
        engine::GameObject* next_level_object_ptr_{ nullptr };

        const bool spawn_enemies_{ false };

        void load_level( const std::string& path, engine::GameObject& level );
        void load_next_level( );
        void load_random_level( );
        void reposition_players( ) const;

        void lower_enemy_count( );

    };

}


#endif //!LEVELLOADERCOMPONENT_H
