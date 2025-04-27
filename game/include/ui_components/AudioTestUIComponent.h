#ifndef AUDIOTESTUICOMPONENT_H
#define AUDIOTESTUICOMPONENT_H

#include <framework/ui_components/UIComponent.h>


namespace game
{
    class AudioTestUIComponent final : public engine::UIComponent
    {
    public:
        explicit AudioTestUIComponent( engine::UIController& owner );

        void render( ) override;

    };

}


#endif //!AUDIOTESTUICOMPONENT_H
