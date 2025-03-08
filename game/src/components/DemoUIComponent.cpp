#include "components/DemoUIComponent.h"

#include "imgui.h"

namespace game
{
	DemoUIComponent::DemoUIComponent( engine::UIController& owner )
		: UIComponent( owner )
	{
	}

	void DemoUIComponent::render( )
	{
		if ( show_demo_window_ )
		{
			ImGui::ShowDemoWindow( &show_demo_window_ );
		}
		else
		{
			ImGui::Begin( "Demo UI Component" );
			if ( ImGui::Button( "Show Demo Window" ) )
			{
				show_demo_window_ = true;
			}
			ImGui::End( );
		}

	}

}
