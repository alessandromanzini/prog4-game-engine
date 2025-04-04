#ifndef BINDINGSTRUCTS_H
#define BINDINGSTRUCTS_H

// +----------------------------+
// | STANDARD HEADERS	        |
// +----------------------------+
#include "bindings/binding_types.h"
#include "framework/UID.h"

// +--------------------------------+
// | GLM HEADERS					|
// +--------------------------------+
#include <glm.hpp>

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <functional>
#include <variant>


namespace engine::binding
{
    // +---------------------------+
    // | INPUT ACTION              |
    // +---------------------------+
    struct InputAction final
    {
        UID uid{ 0 };
        modifier_bitset_t modifiers{};
    };

    // +---------------------------+
    // | INPUT SNAPSHOT            |
    // +---------------------------+
    // This struct holds the accumulated value for the input action to its corresponding uid.
    struct InputSnapshot final
    {
        UID uid{ 0 };
        input_value_variant_t value{};
        trigger_bitset_t triggers{};
    };

    // +---------------------------+
    // | COMMAND INFO              |
    // +---------------------------+
    struct CommandInfo final
    {
        input_command_variant_t command;
        TriggerEvent trigger;
    };

}

#endif //!BINDINGSTRUCTS_H
