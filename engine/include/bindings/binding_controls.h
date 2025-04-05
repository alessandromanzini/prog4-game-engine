#ifndef BINDINGCONTROLS_H
#define BINDINGCONTROLS_H

// +---------------------------+
// | PROJECT HEADERS           |
// +---------------------------+
#include <bindings/binding_types.h>

// +---------------------------+
// | STANDARD HEADERS          |
// +---------------------------+
#include <cassert>


namespace engine::binding
{
    // +---------------------------+
    // | MERGING                   |
    // +---------------------------+
    void merge_value_to_snapshot( InputSnapshot& snapshot, input_value_variant_t value );


    // +--------------------------------+
    // | COMPILE-TIME CASTS				|
    // +--------------------------------+
    // Converts the mask to a sequence number
    [[nodiscard]] constexpr uint32_t mask_to_seq( uint32_t mask ) noexcept
    {
        assert( mask != 0 && "Mask cannot be 0!" );

        uint32_t pos = 0;
        while ( ( mask & 1 ) == 0 )
        {
            mask >>= 1;
            pos++;
        }
        return pos;
    }


    // Casts the sequence enum value to a single bit representation
    [[nodiscard]] constexpr size_t bit_cast( TriggerEvent trigger ) noexcept
    {
        return static_cast<size_t>( trigger );
    }


    // Casts the sequence enum value to a single bit representation
    [[nodiscard]] constexpr size_t bit_cast( ModifierType modifier ) noexcept
    {
        return static_cast<size_t>( modifier );
    }


    // Casts the enum to the bitset representation
    template <typename... args_t>
        requires (std::same_as<args_t, TriggerEvent> && ...)
    [[nodiscard]] constexpr trigger_bitset_t bitset_cast( args_t... triggers ) noexcept
    {
        return trigger_bitset_t{ ( ( 1u << bit_cast( triggers ) ) | ... ) };
    }


    // Casts the enum to the bitset representation
    template <typename... args_t>
        requires (std::same_as<args_t, ModifierType> && ...)
    [[nodiscard]] constexpr modifier_bitset_t bitset_cast( args_t... modifiers ) noexcept
    {
        return modifier_bitset_t{ ( ( 1u << bit_cast( modifiers ) ) | ... ) };
    }


    [[nodiscard]] constexpr bool trigger_to_value( const TriggerEvent trigger )
    {
        switch ( trigger )
        {
            case TriggerEvent::TRIGGERED:
            case TriggerEvent::PRESSED:
                return true;
            case TriggerEvent::RELEASED:
                return false;
            default:
                assert( false && "Can't cast this trigger!" );
        }
    }

}


#endif //!BINDINGCONTROLS_H
