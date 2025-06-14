#ifndef GAMEOVERCOMPONENT_H
#define GAMEOVERCOMPONENT_H

#include <framework/component/Component.h>
#include <framework/event/Observer.h>
#include <framework/resource/Font.h>

#include <memory>
#include <vector>


namespace game
{
    class HighScoresDisplayComponent;
    class GameOverComponent final : public engine::Component, public engine::Observer
    {
    public:
        explicit GameOverComponent( owner_t& owner, const std::shared_ptr<engine::Font>& font, bool showHighScores = true );
        void tick( ) override;
        void notify( engine::UID event, engine::event::broadcast_value_variant_t value ) override;

        void letter_vert( bool up );
        void letter_horz( bool left );
        void confirm( );

    private:
        static constexpr int MAX_HIGH_SCORES_{ 9 };
        const bool show_high_scores_{ false };

        std::shared_ptr<engine::Font> font_ptr_{ nullptr };
        engine::GameObject* gameover_text_object_ptr_{ nullptr };

        bool waiting_for_redirect_{ false };
        bool is_gameover_{ false };

        std::vector<std::pair<std::string, int>> scores_{};
        bool needs_name_input_{ false };
        size_t score_index_{ };
        size_t letter_index_{ };

        void manage_high_score( int score );
        [[nodiscard]] std::vector<std::pair<std::string, int>> get_high_scores( ) const;
        void update_high_scores( const std::vector<std::pair<std::string, int>>& scores ) const;

        void display_scores( );

    };

}


#endif //!GAMEOVERCOMPONENT_H
