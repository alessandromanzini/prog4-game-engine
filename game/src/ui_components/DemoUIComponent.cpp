#include <ui_components/DemoUIComponent.h>

// +--------------------------------+
// | IMGUI HEADERS					|
// +--------------------------------+
#include <imgui.h>


namespace game
{
    DemoUIComponent::DemoUIComponent( engine::UIController& owner )
        : UIComponent( owner ) { }


    void DemoUIComponent::render( )
    {
        // ReSharper disable once CppDFAConstantConditions
        if ( show_demo_window_ )
        {
            // Add source demo window from cmake to the project for this to work
            // ImGui::ShowDemoWindow( &show_demo_window_ );
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
