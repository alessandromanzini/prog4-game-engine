#include "object_stats.h"


namespace game::stats
{
    constexpr float CHARACTER_MOVEMENT_SPEED{ 80.f };
    constexpr float ZEN_CHAN_MOVEMENT_SPEED{ 60.f };
    constexpr float MAITA_MOVEMENT_SPEED{ 50.f };
    constexpr float JUMP_FORCE{ 300.f };


    float get_character_speed( ) { return CHARACTER_MOVEMENT_SPEED; }
    float get_character_jump_force( ) { return JUMP_FORCE; }

    float get_zen_chan_speed( ) { return ZEN_CHAN_MOVEMENT_SPEED; }
    float get_zen_chan_jump_force( ) { return JUMP_FORCE; }

    float get_maita_speed( ) { return MAITA_MOVEMENT_SPEED; }
    float get_maita_jump_force( ) { return JUMP_FORCE; }


    CharacterResources get_bub_resources( )
    {
        return {
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
            .reposition_sprite = engine::Sprite2D{
                "characters/bub/bub_reposition_1x4.png",
                static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 4u ),
                .1f, 2.f
            },
            .attack_audio_path = "characters/sfx/character_attack.wav",
            .jump_audio_path = "characters/sfx/character_jump.wav",
            .bounce_audio_path = "characters/sfx/character_bounce.wav",
        };
    }


    CharacterResources get_bob_resources( )
    {
        return {
            .idle_sprite = engine::Sprite2D{
                "characters/bob/bob_idle_1x8.png",
                static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 8u ),
                0.2f, 2.f
            },
            .walk_sprite = engine::Sprite2D{
                "characters/bob/bob_walk_1x6.png",
                static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 6u ),
                0.2f, 2.f,
                { 0.f, -4.f }
            },
            .rise_sprite = engine::Sprite2D{
                "characters/bob/bob_jump_1x2.png",
                static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 2u ),
                0.2f, 2.f
            },
            .fall_sprite = engine::Sprite2D{
                "characters/bob/bob_fall_1x1.png",
                static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 1u ),
                0.2f, 2.f
            },
            .attack_sprite = engine::Sprite2D{
                "characters/bob/bob_attack_1x3.png",
                static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 3u ),
                .1f, 2.f, {}, false
            },
            .reposition_sprite = engine::Sprite2D{
                "characters/bob/bob_reposition_1x4.png",
                static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 4u ),
                .1f, 2.f
            },
            .attack_audio_path = "characters/sfx/character_attack.wav",
            .jump_audio_path = "characters/sfx/character_jump.wav",
            .bounce_audio_path = "characters/sfx/character_bounce.wav",
        };
    }


    CharacterResources get_zen_chan_resources( )
    {
        return {
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
            .reposition_sprite = engine::Sprite2D{
                "characters/zen_chan/zen_chan_walk_1x4.png",
                static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 4u ),
                0.2f, 2.f
            },
            .attack_audio_path = "characters/sfx/character_attack.wav",
            .jump_audio_path = "characters/sfx/character_jump.wav",
            .bounce_audio_path = "characters/sfx/character_bounce.wav",
        };
    }


    BubbleCaptureResources get_zen_chan_capture_resources( )
    {
        return {
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
    }


    CharacterResources get_maita_resources( )
    {
        return {
            .idle_sprite = engine::Sprite2D{
                "characters/maita/maita_walk_1x2.png",
                static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 2u ),
                0.3f, 2.f
            },
            .walk_sprite = engine::Sprite2D{
                "characters/maita/maita_walk_1x2.png",
                static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 2u ),
                0.2f, 2.f
            },
            .rise_sprite = engine::Sprite2D{
                "characters/maita/maita_fall_1x3.png",
                static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 3u ),
                0.2f, 2.f
            },
            .fall_sprite = engine::Sprite2D{
                "characters/maita/maita_fall_1x3.png",
                static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 3u ),
                0.2f, 2.f
            },
            .attack_sprite = engine::Sprite2D{
                "characters/maita/maita_attack_1x5.png",
                static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 5u ),
                0.3f, 2.f, { -14.f, 0.f }
            },
            .reposition_sprite = engine::Sprite2D{
                "characters/maita/maita_walk_1x2.png",
                static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 2u ),
                0.3f, 2.f
            },
            .attack_audio_path = "characters/sfx/character_attack.wav",
            .jump_audio_path = "characters/sfx/character_jump.wav",
            .bounce_audio_path = "characters/sfx/character_bounce.wav",
        };
    }


    BubbleCaptureResources get_maita_capture_resources( )
    {
        return {
            .green_bubble_sprite = engine::Sprite2D{
                "characters/maita/maita_bubble_green_1x3.png",
                static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 3u ),
                0.2f, 2.f
            },
            .blue_bubble_sprite = engine::Sprite2D{
                "characters/maita/maita_bubble_blue_1x3.png",
                static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 3u ),
                0.2f, 2.f
            },
            .yellow_bubble_sprite = engine::Sprite2D{
                "characters/maita/maita_bubble_yellow_1x3.png",
                static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 3u ),
                0.2f, 2.f
            },
            .red_bubble_sprite = engine::Sprite2D{
                "characters/maita/maita_bubble_red_1x3.png",
                static_cast<uint8_t>( 1u ), static_cast<uint8_t>( 3u ),
                0.2f, 2.f
            },
            .fruit_texture_path = "characters/maita/maita_fruit.png",
            .fruit_value = 200
        };
    }

}
