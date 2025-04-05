#ifndef BINDINGTYPES_H
#define BINDINGTYPES_H

// +---------------------------+
// | PROJECT HEADERS           |
// +---------------------------+
#include <framework/UID.h>

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
        UniformBindingCode( ) = default;
        constexpr UniformBindingCode( const key_t key ) : code{ static_cast<decltype(code)>( key ) } { }
        constexpr UniformBindingCode( const btn_t btn ) : code{ static_cast<decltype(code)>( btn ) } { }

        explicit operator uint32_t( ) const { return code; }

        [[nodiscard]] bool operator<( const UniformBindingCode& other ) const noexcept { return code < other.code; }
        [[nodiscard]] bool operator==( const UniformBindingCode& other ) const noexcept { return code == other.code; }

        uint32_t code{};

    };

    struct UniformBindingCodeHasher final
    {
        std::size_t operator()( const UniformBindingCode& ubc ) const noexcept
        {
            return std::hash<uint32_t>{}( ubc.code );
        }
    };


    // +--------------------------------+
    // | ENUMERATIONS					|
    // +--------------------------------+
    namespace detail
    {
        using trigger_mask_t  = uint8_t;
        using modifier_mask_t = uint8_t;
    }

    enum class TriggerEvent : detail::trigger_mask_t
    {
        TRIGGERED = 0x0,
        PRESSED   = 0x1,
        RELEASED  = 0x2,
    };

    enum class ModifierType : detail::modifier_mask_t
    {
        NEGATE  = 0x0,
        SWIZZLE = 0x1,
    };

    enum class DeviceType : uint8_t
    {
        KEYBOARD,
        GAMEPAD
    };


    // +--------------------------------+
    // | BITSET TYPES					|
    // +--------------------------------+
    using trigger_bitset_t  = std::bitset<sizeof( detail::trigger_mask_t ) * 8>;
    using modifier_bitset_t = std::bitset<sizeof( detail::modifier_mask_t ) * 8>;


    // +--------------------------------+
    // | VARIANTS						|
    // +--------------------------------+
    using input_value_variant_t   = std::variant<bool, float /*, glm::vec2 */>;
    using input_command_variant_t = std::variant<std::function<void( bool )>, std::function<void( float )>
        /*, std::function<void( glm::vec2 )>*/>;


    // +---------------------------+
    // | STRUCTS                   |
    // +---------------------------+
    struct InputAction final
    {
        UID uid{ 0 };
        modifier_bitset_t modifiers{};
    };

    // This struct holds the accumulated value for the input action to its corresponding uid.
    struct InputSnapshot final
    {
        UID uid{ 0 };
        input_value_variant_t value{};
        trigger_bitset_t triggers{};
    };

    struct CommandInfo final
    {
        input_command_variant_t command;
        TriggerEvent trigger;
    };

}

#endif //!BINDINGTYPES_H
