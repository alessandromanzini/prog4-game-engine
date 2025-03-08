#ifndef DEMOUICOMPONENT_H
#define DEMOUICOMPONENT_H

#include "UIComponent.h"

namespace game
{
	class DemoUIComponent final : public engine::UIComponent
	{
	public:
		DemoUIComponent( engine::UIController& owner );

		void render( ) override;

	private:
		bool show_demo_window_{ true };

	};

}

#endif // !DEMOUICOMPONENT_H
