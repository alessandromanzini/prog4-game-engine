#include "components/PlayerControllerComponent.h"

#include "InputUtils.h"

#include "bindings/AttackCommand.h"
#include "bindings/ScoreCommand.h"


namespace game
{
    PlayerControllerComponent::PlayerControllerComponent( engine::GameObjectView& owner, bool gamepad )
        : engine::Component{ owner }
        , attack_command_{ std::make_unique<AttackCommand>( owner.get_component<AttackComponent>( ) ) }
        , score_up_command_{ std::make_unique<ScoreCommand>( owner.get_component<ScoreComponent>( ), 10 ) }
        , score_great_up_command_{ std::make_unique<ScoreCommand>( owner.get_component<ScoreComponent>( ), 100 ) }
    {
        auto& imc{ engine::INPUT_SYSTEM.get_input_mapping_context( ) };

        // I know that switching here between keyboard and gamepad is horrendous.
        // I'm working on it :-)
        if ( gamepad )
        {
            imc.bind_input_action( engine::UID( IA::GATTACK ), attack_command_.get( ) );
            imc.bind_input_action( engine::UID( IA::GSCORE_UP ), score_up_command_.get( ) );
            imc.bind_input_action( engine::UID( IA::GSCORE_GREAT_UP ), score_great_up_command_.get( ) );
        }
        else
        {
            imc.bind_input_action( engine::UID( IA::KATTACK ), attack_command_.get( ) );
            imc.bind_input_action( engine::UID( IA::KSCORE_UP ), score_up_command_.get( ) );
            imc.bind_input_action( engine::UID( IA::KSCORE_GREAT_UP ), score_great_up_command_.get( ) );
        }
    }


    PlayerControllerComponent::~PlayerControllerComponent( ) noexcept = default;

}
