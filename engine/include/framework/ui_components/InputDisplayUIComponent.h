#ifndef INPUTDISPLAYUICOMPONENT_H
#define INPUTDISPLAYUICOMPONENT_H

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include "UIComponent.h"


namespace engine
{
    class InputDisplayUIComponent final : public UIComponent
    {
    public:
        explicit InputDisplayUIComponent( UIController& owner );

        void render( ) override;

    };

}

#endif //!INPUTDISPLAYUICOMPONENT_H
