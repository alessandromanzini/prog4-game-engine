#ifndef BINDINGCONTROLS_H
#define BINDINGCONTROLS_H

// +---------------------------+
// | PROJECT HEADERS           |
// +---------------------------+
#include <bindings/binding_structs.h>
#include <bindings/binding_types.h>


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


    // Casts the sequence enum value to a bitset representation
    [[nodiscard]] constexpr uint32_t seq_mask_cast( TriggerEvent trigger ) noexcept
    {
        return mask_to_seq( static_cast<uint32_t>( trigger ) );
    }


    [[nodiscard]] constexpr uint32_t seq_mask_cast( ModifierType modifier ) noexcept
    {
        return mask_to_seq( static_cast<uint32_t>( modifier ) );
    }


    // Casts the enum to the bitset representation
    [[nodiscard]] constexpr trigger_bitset_t bitset_cast( TriggerEvent trigger ) noexcept
    {
        return static_cast<trigger_mask_t>( trigger );
    }


    [[nodiscard]] constexpr modifier_bitset_t bitset_cast( ModifierType modifier ) noexcept
    {
        return static_cast<modifier_mask_t>( modifier );
    }


    [[nodiscard]] constexpr bool trigger_to_value( TriggerEvent trigger )
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
