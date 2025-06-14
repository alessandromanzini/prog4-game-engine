#ifndef GAMEOVERCOMPONENT_H
#define GAMEOVERCOMPONENT_H

#include <framework/component/Component.h>
#include <framework/event/Observer.h>
#include <framework/resource/Font.h>


namespace game
{
    class GameOverComponent final : public engine::Component, public engine::Observer
    {
    public:
        explicit GameOverComponent( owner_t& owner, const std::shared_ptr<engine::Font>& font );
        void notify( engine::UID event, engine::event::broadcast_value_variant_t value ) override;

    private:
        engine::GameObject* gameover_text_object_ptr_{ nullptr };
        bool is_gameover_{ false };

        [[nodiscard]] bool is_high_score( int score ) const;
        void update_high_score( int score ) const;

    };

}


#endif //!GAMEOVERCOMPONENT_H
