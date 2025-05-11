#include <ui_components/AudioTestUIComponent.h>

#include <SDL_mixer.h>

#include <imgui.h>
#include <singletons/ServiceLocator.h>


namespace game
{
    AudioTestUIComponent::AudioTestUIComponent( engine::UIController& owner )
        : UIComponent( owner )
    {

    }


    void AudioTestUIComponent::render( )
    {
        auto& ss = engine::SERVICE_LOCATOR.get_sound_system(  );

        ImGui::Begin( "Audio Test" );

        // Header styling
        ImGui::SeparatorText("Controls");

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 6));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 6));
        ImGui::BeginGroup();
        ImGui::Indent();
        {
            ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(30, 30, 30, 100));
            ImGui::BeginChild("ControlsBox", ImVec2(0, 100), true, ImGuiWindowFlags_AlwaysUseWindowPadding);
            // ImGui::Text( "F: Play sound" );
            // ImGui::Text( "UP ARROW: Volume up" );
            // ImGui::Text( "DOWN ARROW: Volume down" );
            // ImGui::Spacing();
            // ImGui::Text( "K: Kill pacman" );
            ImGui::Text( "A: Jump" );
            ImGui::Text( "D-PAD: Walk" );
            ImGui::EndChild();
            ImGui::PopStyleColor();
        }
        ImGui::Unindent();
        ImGui::EndGroup();
        ImGui::PopStyleVar(2);

        ImGui::Spacing();
        ImGui::SeparatorText("Service");

        ImGui::Indent( );
        {
            ImGui::Text( "Master Volume:" );
            ImGui::SameLine(  );
            ImGui::Text( "%s", std::to_string( ss.get_master_volume( ) ).c_str(  ) );

            ImGui::Spacing( );

            for ( int i{}; i < 2; ++i )
            {
                std::string channelName = "Channel " + std::to_string( i ) + ":";
                ImGui::Text( "%s", channelName.c_str( ) );
                ImGui::SameLine(  );
                if ( Mix_Playing( i ) )
                {
                    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Playing");
                }
                else
                {
                    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Idle");
                }
            }
        }
        ImGui::Unindent(  );

        ImGui::Spacing();
        ImGui::SeparatorText("Details");

        ImGui::Indent( );
        {
            ImGui::TextWrapped( "Everytime a resource owner is destroyed, the resource manager will call unload resource through its subject to cleanup resources." );
        }
        ImGui::Unindent( );

        ImGui::End( );
    }

}
