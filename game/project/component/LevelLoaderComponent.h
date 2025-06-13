#ifndef LEVELLOADERCOMPONENT_H
#define LEVELLOADERCOMPONENT_H

#include <vec2.hpp>
#include <framework/component/Component.h>

#include <vector>


namespace game
{
    class LevelLoaderComponent final : public engine::Component
    {
    public:
        explicit LevelLoaderComponent( owner_t& owner, engine::GameObject& score, std::vector<engine::GameObject*> players,
                                       std::vector<const char*> levelPaths );
        void tick( ) override;

    private:
        static constexpr float LEVEL_TRANSITION_SPEED_{ 200.f };

        const std::vector<const char*> level_paths_{};
        engine::GameObject& score_object_ref_;
        std::vector<engine::GameObject*> players_{};

        uint32_t current_level_{ 0 };
        uint32_t enemy_count_{ 0 };
        engine::GameObject* current_level_object_ptr_{ nullptr };
        engine::GameObject* next_level_object_ptr_{ nullptr };

        void load_level( const std::string& path, engine::GameObject& level );
        void reposition_players( ) const;

        void lower_enemy_count( );
        void load_next_level( );

    };

}


#endif //!LEVELLOADERCOMPONENT_H
