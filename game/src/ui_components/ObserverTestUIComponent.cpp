#include "ui_components/ObserverTestUIComponent.h"

// +--------------------------------+
// | IMGUI HEADERS					|
// +--------------------------------+
#include "imgui.h"


namespace game
{
    ObserverTestUIComponent::ObserverTestUIComponent( engine::UIController& owner, bool gamepad )
        : UIComponent( owner )
        , Observer( )
        , gamepad_( gamepad )
    {
        if ( gamepad_ )
        {
            controls_text_ = "PacMan: Use X to inflict damage, A and B to gain score.";
        }
        else
        {
            controls_text_ = "Ms PacMan: Use C to inflict damage, Z and X to gain score.";
        }
    }


    void ObserverTestUIComponent::render( )
    {
        if ( gamepad_ )
        {
            ImGui::Begin( "GamePad Observer Test" );
        }
        else
        {
            ImGui::Begin( "Keyboard Observer Test" );
        }

        ImGui::Text( "Controls" );
        ImGui::Spacing( );
        ImGui::Separator( );
        //ImGui::Text( controls_text_.c_str( ) );

        ImGui::Spacing( );
        ImGui::Separator( );
        ImGui::Spacing( );

        ImGui::Text( "Player Info" );
        ImGui::Spacing( );
        ImGui::Text( "Player Health: %d", player_health_ );
        ImGui::Text( "Player Score:  %d", player_score_ );

        ImGui::End( );
    }


    void ObserverTestUIComponent::notify( engine::UID event, engine::Subject* /* subject */,
                                          engine::event::broadcast_value_variant_t value )
    {
        if ( event == engine::UID( "ActorHealthChanged" ) )
        {
            player_health_ = std::get<int>( value );
        }
        else if ( event == engine::UID( "ActorScoreChanged" ) )
        {
            player_score_ = std::get<int>( value );
        }
    }

}
