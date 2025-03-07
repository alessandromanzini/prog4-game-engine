#ifndef DEMOUICOMPONENT_H
#define DEMOUICOMPONENT_H

#include "BaseUIComponent.h"

namespace game
{
	class DemoUIComponent final : public engine::BaseUIComponent
	{
	public:
		DemoUIComponent( engine::UIController& owner );

		void render( ) override;

	private:
		bool show_demo_window_{ true };

	};

}

#endif // !DEMOUICOMPONENT_H
