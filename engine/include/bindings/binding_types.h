#ifndef BINDINGTYPES_H
#define BINDINGTYPES_H

// +--------------------------------+
// | SDL HEADERS	    			|
// +--------------------------------+
#include <SDL_gamecontroller.h>
#include <SDL_keycode.h>

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <bitset>
#include <functional>


namespace engine::binding
{
    // +--------------------------------+
    // | KEY/BTN TYPES					|
    // +--------------------------------+
    using key_t = SDL_KeyCode;
    using btn_t = SDL_GameControllerButton;

    // +---------------------------+
    // | UNIFORM BINDING CODE      |
    // +---------------------------+
    // This struct generalizes the key binding values
    struct UniformBindingCode final
    {
        UniformBindingCode( ) noexcept = default;
        constexpr UniformBindingCode( const key_t key ) : code{ static_cast<decltype(code)>( key ) } { }
        constexpr UniformBindingCode( const btn_t btn ) : code{ static_cast<decltype(code)>( btn ) } { }

        explicit operator uint32_t( ) const { return code; }

        [[nodiscard]] bool operator<( const UniformBindingCode& other ) const { return code < other.code; }
        [[nodiscard]] bool operator==( const UniformBindingCode& other ) const { return code == other.code; }

        uint32_t code{};

    };

    struct UniformBindingCodeHasher
    {
        std::size_t operator()( const UniformBindingCode& ubc ) const noexcept
        {
            return std::hash<uint32_t>{}( ubc.code );
        }
    };

    // +--------------------------------+
    // | ENUMERATIONS					|
    // +--------------------------------+
    using trigger_mask_t = uint8_t;
    enum class TriggerEvent : trigger_mask_t
    {
        TRIGGERED = 0x1,
        PRESSED   = 0x2,
        RELEASED  = 0x3,

        ALL = 0xF
    };

    using modifier_mask_t = uint8_t;
    enum class ModifierType : modifier_mask_t
    {
        NEGATE  = 0x1,
        SWIZZLE = 0x2,

        ALL = 0xF
    };

    enum class DeviceType : uint8_t
    {
        KEYBOARD,
        GAMEPAD
    };

    // +--------------------------------+
    // | BITSET TYPES					|
    // +--------------------------------+
    using trigger_bitset_t  = std::bitset<sizeof( trigger_mask_t ) * 8>;
    using modifier_bitset_t = std::bitset<sizeof( modifier_mask_t ) * 8>;

    // +--------------------------------+
    // | VARIANTS						|
    // +--------------------------------+
    using input_value_variant_t   = std::variant<bool, float /*, glm::vec2 */>;
    using input_command_variant_t = std::variant<std::function<void( bool )>, std::function<void( float )>
        /*, std::function<void( glm::vec2 )>*/>;

}

#endif //!BINDINGTYPES_H
