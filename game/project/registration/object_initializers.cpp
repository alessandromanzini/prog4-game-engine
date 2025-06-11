#include "object_initializers.h"

#include <component/BubbleCaptureComponent.h>
#include <framework/GameObject.h>
#include <framework/component/TextComponent.h>
#include <framework/component/physics/BoxColliderComponent.h>
#include <framework/component/physics/PhysicsComponent.h>

#include <singleton/ResourceManager.h>
#include <singleton/ServiceLocator.h>

#include <registration/tags.h>

#include <component/CharacterComponent.h>

#include <component/CollisionsComponent.h>
#include <component/FruitComponent.h>
#include <component/ZenChanComponent.h>
#include <framework/component/TextureComponent.h>
#include <framework/component/AudioComponent.h>

#include "audio.h"


namespace game
{
    void create_bub( engine::GameObject& object, const glm::vec2 position )
    {
        constexpr float PROJECTILE_SPEED{ 100.f };
        constexpr float PROJECTILE_OFFSET{ 20.f };
        constexpr float JUMP_FORCE{ 300.f };
        constexpr float MOVEMENT_SPEED{ 80.f };

        object.set_tag( engine::UID( ObjectTags::ALLY ) );
        object.set_world_transform( engine::Transform::from_translation( position ) );

        auto& body = object.add_component<engine::PhysicsComponent>( );
        body.set_simulate_physics( true );

        object.add_component<engine::BoxColliderComponent>( glm::vec2{ 24.f, 40.f }, glm::vec2{ 8.f, 0.f } );
        object.add_component<CollisionsComponent>( );

        CharacterResources resources{
            .idle_sprite = engine::Sprite2D{
                "characters/bub/bub_idle_1x8.png",
                static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 8u ),
                0.2f, 2.f
            },
            .walk_sprite = engine::Sprite2D{
                "characters/bub/bub_walk_1x6.png",
                static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 6u ),
                0.2f, 2.f,
                { 0.f, -4.f }
            },
            .rise_sprite = engine::Sprite2D{
                "characters/bub/bub_jump_1x2.png",
                static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 2u ),
                0.2f, 2.f
            },
            .fall_sprite = engine::Sprite2D{
                "characters/bub/bub_fall_1x1.png",
                static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 1u ),
                0.2f, 2.f
            },
            .attack_sprite = engine::Sprite2D{
                "characters/bub/bub_attack_1x3.png",
                static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 3u ),
                .1f, 2.f, {}, false
            },
            .attack_audio_path = "characters/sfx/character_attack.wav",
            .jump_audio_path = "characters/sfx/character_jump.wav",
            .bounce_audio_path = "characters/sfx/character_bounce.wav",
        };

        auto& projectileSocket = object.create_child( );
        projectileSocket.set_local_transform( engine::Transform::from_translation( { 20.f, 20.f } ) );

        object.add_component<CharacterComponent>( resources,
                                                  std::make_unique<AttackCommand>(
                                                      object, projectileSocket, PROJECTILE_SPEED, PROJECTILE_OFFSET ),
                                                  std::make_unique<JumpCommand>( object, JUMP_FORCE ),
                                                  std::make_unique<MoveCommand>( object, MOVEMENT_SPEED ) );
    }


    void create_zenchan( engine::GameObject& object, const glm::vec2 position, std::vector<engine::GameObject*> targets )
    {
        constexpr float JUMP_FORCE{ 280.f };
        constexpr float MOVEMENT_SPEED{ 60.f };

        object.set_tag( engine::UID( ObjectTags::ENEMY ) );
        object.set_world_transform( engine::Transform::from_translation( position ) );

        auto& body = object.add_component<engine::PhysicsComponent>( );
        body.set_simulate_physics( true );

        object.add_component<engine::BoxColliderComponent>( glm::vec2{ 24.f, 32.f }, glm::vec2{ 4.f, 0.f } );
        object.add_component<CollisionsComponent>( );

        CharacterResources resources{
            .idle_sprite = engine::Sprite2D{
                "characters/zen_chan/zen_chan_walk_1x4.png",
                static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 4u ),
                0.2f, 2.f
            },
            .walk_sprite = engine::Sprite2D{
                "characters/zen_chan/zen_chan_walk_1x4.png",
                static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 4u ),
                0.2f, 2.f
            },
            .rise_sprite = engine::Sprite2D{
                "characters/zen_chan/zen_chan_walk_1x4.png",
                static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 4u ),
                0.2f, 2.f
            },
            .fall_sprite = engine::Sprite2D{
                "characters/zen_chan/zen_chan_walk_1x4.png",
                static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 4u ),
                0.2f, 2.f
            },
            .attack_sprite = engine::Sprite2D{
                "characters/zen_chan/zen_chan_walk_1x4.png",
                static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 4u ),
                0.2f, 2.f
            },
            .attack_audio_path = "characters/sfx/character_attack.wav",
            .jump_audio_path = "characters/sfx/character_jump.wav",
            .bounce_audio_path = "characters/sfx/character_bounce.wav",
        };

        object.add_component<CharacterComponent>( resources,
                                                  std::make_unique<AttackCommand>( object, object, 0.f ),
                                                  std::make_unique<JumpCommand>( object, JUMP_FORCE ),
                                                  std::make_unique<MoveCommand>( object, MOVEMENT_SPEED ) );

        BubbleCaptureResources bubbleResources{
            .green_bubble_sprite = engine::Sprite2D{
                "characters/zen_chan/zen_chan_bubble_green_1x3.png",
                static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 3u ),
                0.2f, 2.f
            },
            .blue_bubble_sprite = engine::Sprite2D{
                "characters/zen_chan/zen_chan_bubble_blue_1x3.png",
                static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 3u ),
                0.2f, 2.f
            },
            .yellow_bubble_sprite = engine::Sprite2D{
                "characters/zen_chan/zen_chan_bubble_yellow_1x3.png",
                static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 3u ),
                0.2f, 2.f
            },
            .red_bubble_sprite = engine::Sprite2D{
                "characters/zen_chan/zen_chan_bubble_red_1x3.png",
                static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 3u ),
                0.2f, 2.f
            },
            .fruit_texture_path = "characters/zen_chan/zen_chan_fruit.png",
            .fruit_value = 100
        };
        object.add_component<BubbleCaptureComponent>( bubbleResources );
        object.add_component<ZenChanComponent>(  ).set_targets( std::move( targets ) );
    }


    void create_fruit( engine::GameObject& object, const std::string& texturePath, int value, const glm::vec2 position )
    {
        object.set_world_transform( engine::Transform::from_translation( position ) );
        object.set_tag( engine::UID( ObjectTags::FRUIT ) );

        object.add_component<engine::TextureComponent>( texturePath );
        object.add_component<engine::BoxColliderComponent>( glm::vec2{ 36.f, 30.f } );
        object.add_component<CollisionsComponent>( );
        object.add_component<FruitComponent>( value, 5 );
        object.add_component<engine::AudioComponent>( "characters/sfx/fruit_capture.wav",
                                                      engine::sound::SoundType::SOUND_EFFECT,
                                                      engine::UID( AudioCue::SFX ) ).set_playback_on_deletion( );
    }


    void create_grid( engine::GameObject& object )
    {
        object.add_component<engine::TextureComponent>( "maps/level1_layout.png" );
        object.add_component<engine::BoxColliderComponent>( glm::vec2{ 640.f, 25.f } );
        object.add_component<engine::BoxColliderComponent>( glm::vec2{ 50.f, 480.f } );
        object.add_component<engine::BoxColliderComponent>( glm::vec2{ 640.f, 25.f }, glm::vec2{ 0.f, 465.f } );
        object.add_component<engine::BoxColliderComponent>( glm::vec2{ 50.f, 480.f }, glm::vec2{ 600.f, 0.f } );

        auto& platformsObject = object.create_child( );
        platformsObject.set_tag( engine::UID( ObjectTags::PLATFORM ) );
        platformsObject.add_component<engine::BoxColliderComponent>( glm::vec2{ 40.f, 20.f }, glm::vec2{ 40.f, 370.f } );
        platformsObject.add_component<engine::BoxColliderComponent>( glm::vec2{ 360.f, 20.f }, glm::vec2{ 140.f, 370.f } );
        platformsObject.add_component<engine::BoxColliderComponent>( glm::vec2{ 40.f, 20.f }, glm::vec2{ 560.f, 370.f } );

        platformsObject.add_component<engine::BoxColliderComponent>( glm::vec2{ 40.f, 20.f }, glm::vec2{ 40.f, 279.f } );
        platformsObject.add_component<engine::BoxColliderComponent>( glm::vec2{ 360.f, 20.f }, glm::vec2{ 140.f, 279.f } );
        platformsObject.add_component<engine::BoxColliderComponent>( glm::vec2{ 40.f, 20.f }, glm::vec2{ 560.f, 279.f } );

        platformsObject.add_component<engine::BoxColliderComponent>( glm::vec2{ 40.f, 20.f }, glm::vec2{ 40.f, 185.f } );
        platformsObject.add_component<engine::BoxColliderComponent>( glm::vec2{ 360.f, 20.f }, glm::vec2{ 140.f, 185.f } );
        platformsObject.add_component<engine::BoxColliderComponent>( glm::vec2{ 40.f, 20.f }, glm::vec2{ 560.f, 185.f } );
    }

}
