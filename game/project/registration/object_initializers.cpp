#include "object_initializers.h"

#include <command/BubbleAttackCommand.h>
#include <command/EmptyAttackCommand.h>
#include <command/RockAttackCommand.h>
#include <component/BubbleCaptureComponent.h>
#include <framework/GameObject.h>
#include <framework/component/physics/BoxColliderComponent.h>
#include <framework/component/physics/PhysicsComponent.h>

#include <singleton/ResourceManager.h>

#include <registration/tags.h>

#include <component/CharacterComponent.h>

#include <component/CollisionsComponent.h>
#include <component/FruitComponent.h>
#include <component/MaitaComponent.h>
#include <component/ScoreComponent.h>
#include <component/ScoreDelegateComponent.h>
#include <component/StatsDisplayComponent.h>
#include <component/ZenChanComponent.h>
#include <framework/component/TextureComponent.h>
#include <framework/component/AudioComponent.h>

#include "audio.h"
#include "object_stats.h"


namespace game
{
    void create_character( engine::GameObject& object, ScoreComponent* score, const bool isBub )
    {
        object.set_tag( engine::UID( ObjectTags::ALLY ) );

        auto& body = object.add_component<engine::PhysicsComponent>( );
        body.set_simulate_physics( true );

        object.add_component<engine::BoxColliderComponent>( glm::vec2{ 24.f, 40.f }, glm::vec2{ 8.f, 0.f } );
        object.add_component<CollisionsComponent>( );

        auto& projectileSocket = object.create_child( );
        projectileSocket.set_local_transform( engine::Transform::from_translation( { 20.f, 22.f } ) );

        object.add_component<CharacterComponent>( isBub ? stats::get_bub_resources( ) : stats::get_bob_resources( ),
                                                  std::make_unique<BubbleAttackCommand>( object, projectileSocket ),
                                                  std::make_unique<JumpCommand>( object, stats::get_character_jump_force(  ) ),
                                                  std::make_unique<MoveCommand>( object, stats::get_character_speed(  ) ),
                                                  true );
        object.add_component<ScoreDelegateComponent>( score );
    }

    void create_bub( engine::GameObject& object, ScoreComponent* score )
    {
        create_character( object, score, true );
    }


    void create_bob( engine::GameObject& object, ScoreComponent* score )
    {
        create_character( object, score, false );
    }


    void create_zenchan( engine::GameObject& object, const glm::vec2 position, std::vector<engine::GameObject*> targets )
    {
        object.set_tag( engine::UID( ObjectTags::ENEMY ) );
        object.set_local_transform( engine::Transform::from_translation( position ) );

        auto& body = object.add_component<engine::PhysicsComponent>( );
        body.set_simulate_physics( true );

        object.add_component<engine::BoxColliderComponent>( glm::vec2{ 24.f, 32.f }, glm::vec2{ 4.f, 0.f } );
        object.add_component<CollisionsComponent>( );

        object.add_component<CharacterComponent>( stats::get_zen_chan_resources( ),
                                                  std::make_unique<EmptyAttackCommand>( object ),
                                                  std::make_unique<JumpCommand>( object, stats::get_zen_chan_jump_force( ) ),
                                                  std::make_unique<MoveCommand>( object, stats::get_zen_chan_speed( ) ),
                                                  false );
        object.add_component<BubbleCaptureComponent>( stats::get_zen_chan_capture_resources( ) );
        object.add_component<ZenChanComponent>( ).set_targets( std::move( targets ) );
    }


    void create_maita( engine::GameObject& object, const glm::vec2 position, std::vector<engine::GameObject*> targets )
    {
        object.set_tag( engine::UID( ObjectTags::ENEMY ) );
        object.set_local_transform( engine::Transform::from_translation( position ) );

        auto& body = object.add_component<engine::PhysicsComponent>( );
        body.set_simulate_physics( true );

        object.add_component<engine::BoxColliderComponent>( glm::vec2{ 24.f, 32.f }, glm::vec2{ 4.f, 0.f } );
        object.add_component<CollisionsComponent>( );

        auto& projectileSocket = object.create_child( );
        projectileSocket.set_local_transform( engine::Transform::from_translation( { 20.f, 10.f } ) );

        object.add_component<CharacterComponent>( stats::get_maita_resources( ),
                                                  std::make_unique<RockAttackCommand>( object, projectileSocket ),
                                                  std::make_unique<JumpCommand>( object, stats::get_maita_jump_force( ) ),
                                                  std::make_unique<MoveCommand>( object, stats::get_maita_speed( ) ),
                                                  false );
        object.add_component<BubbleCaptureComponent>( stats::get_maita_capture_resources( ) );
        object.add_component<MaitaComponent>(  ).set_targets( std::move( targets ) );
    }


    void create_fruit( engine::GameObject& object, const std::string& texturePath, int value, const glm::vec2 position )
    {
        object.set_local_transform( engine::Transform::from_translation( position ) );
        object.set_tag( engine::UID( ObjectTags::FRUIT ) );

        object.add_component<engine::TextureComponent>( texturePath );
        object.add_component<engine::BoxColliderComponent>( glm::vec2{ 36.f, 30.f } );
        object.add_component<CollisionsComponent>( );
        object.add_component<FruitComponent>( value, 5 );
        object.add_component<engine::AudioComponent>( "characters/sfx/fruit_capture.wav",
                                                      engine::sound::SoundType::SOUND_EFFECT,
                                                      engine::UID( AudioCue::SFX ) ).set_playback_on_deletion( );
    }


    void create_score( engine::GameObject& object, std::shared_ptr<engine::Font> font )
    {
        auto& display = object.add_component<StatsDisplayComponent>( font );

        auto& score = object.add_component<ScoreComponent>( );
        score.add_observer( display );
    }

}
