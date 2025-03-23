#ifndef DEVICECONTEXT_H
#define DEVICECONTEXT_H

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include "bindings/InputAction.h"
#include "framework/UID.h"

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <functional>
#include <list>
#include <optional>
#include <unordered_map>
#include <vector>


namespace engine
{
    class DeviceContext {
    public:
        [[nodiscard]] std::optional<std::reference_wrapper<const std::vector<InputActionBinding>>> get_bindings( uint32_t key ) const;
        [[nodiscard]] std::vector<InputActionBinding>& get_or_create_bindings( uint32_t key );

        // Checks if there is a binding for the given uid and key.
        [[nodiscard]] bool has_bindings( UID uid, uint32_t key ) const;
        // Checks if there is a binding for the given uid on any key.
        [[nodiscard]] bool has_bindings( UID uid ) const;

    private:
        std::unordered_map<uint32_t, std::vector<InputActionBinding>> action_binds_{};

        [[nodiscard]] static bool contains_uid_input_action( UID uid, const std::vector<InputActionBinding>& actions );

    };

}


#endif //!DEVICECONTEXT_H
