#ifndef DEMOUICOMPONENT_H
#define DEMOUICOMPONENT_H

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <ui_components/UIComponent.h>


namespace game
{
    class DemoUIComponent final : public engine::UIComponent
    {
    public:
        explicit DemoUIComponent( engine::UIController& owner );

        void render( ) override;

    private:
        bool show_demo_window_{ true };

    };

}


#endif //!DEMOUICOMPONENT_H
