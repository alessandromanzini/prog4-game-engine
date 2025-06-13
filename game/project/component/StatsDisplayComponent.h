#ifndef STATSDISPLAYCOMPONENT_H
#define STATSDISPLAYCOMPONENT_H

#include <framework/component/Component.h>

#include <framework/event/Observer.h>
#include <framework/resource/Font.h>

#include <memory>


namespace game
{
    class StatsDisplayComponent final : public engine::Component, public engine::Observer
    {
    public:
        StatsDisplayComponent( owner_t& owner, std::shared_ptr<engine::Font> font );
        void notify( engine::UID event, engine::Subject* subject, engine::event::broadcast_value_variant_t value ) override;

    private:
        std::shared_ptr<engine::Font> font_ptr_{ nullptr };
        engine::GameObject* score_text_object_ptr_{ nullptr };
        engine::GameObject* lives_text_object_ptr_{ nullptr };

        void update_score_text( int score ) const;
        void update_lives_text( int lives ) const;

    };

}


#endif //!STATSDISPLAYCOMPONENT_H
