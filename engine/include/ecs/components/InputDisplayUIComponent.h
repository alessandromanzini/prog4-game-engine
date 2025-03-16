#ifndef INPUTDISPLAYUICOMPONENT_H
#define INPUTDISPLAYUICOMPONENT_H

#include "UIComponent.h"

namespace engine
{
	class InputDisplayUIComponent final : public UIComponent
	{
	public:
		InputDisplayUIComponent( UIController& owner );

		void render( ) override;

	};

}

#endif // !INPUTDISPLAYUICOMPONENT_H
