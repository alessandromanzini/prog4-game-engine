#ifndef OBSERVERTESTUICOMPONENT_H
#define OBSERVERTESTUICOMPONENT_H

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <event/Observer.h>
#include <framework/ui_components/UIComponent.h>


namespace game
{
    class ObserverTestUIComponent final : public engine::UIComponent, public engine::Observer
    {
    public:
        ObserverTestUIComponent( engine::UIController& owner, bool gamepad );

        void render( ) override;

        void notify( engine::UID event, engine::Subject* subject,
                     engine::event::broadcast_value_variant_t value ) override;

    private:
        std::string controls_text_{};

        int player_health_{};
        int player_score_{};

        bool gamepad_{};

    };

}


#endif //!OBSERVERTESTUICOMPONENT_H
