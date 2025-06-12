#ifndef JOINMENUCOMPONENT_H
#define JOINMENUCOMPONENT_H

#include <framework/component/Component.h>

#include <framework/binding/binding_device.h>

#include <vector>
#include <array>
#include <optional>


namespace engine
{
    class GameObject;
}

namespace game
{
    class JoinMenuComponent final : public engine::Component
    {
    public:
        explicit JoinMenuComponent( owner_t& owner, engine::GameObject* selectionObject, std::vector<engine::GameObject*>&& playerObjects );

        bool join( engine::binding::DeviceInfo device );
        void leave( uint8_t id );

        void next_selection( );
        void prev_selection( );
        void confirm_selection( );

    private:
        engine::GameObject* selection_object_ptr_{ nullptr };
        engine::GameObject& selector_arrow_ref_;
        uint8_t selection_index_{ 0 };

        std::vector<engine::GameObject*> player_objects_{};

        std::array<std::optional<engine::binding::DeviceInfo>, 2> slot_devices_{};

        void arrange_ui( engine::GameObject* object, const std::optional<engine::binding::DeviceInfo>& info ) const;
        void move_selector( engine::GameObject* target ) const;

    };

}


#endif //!JOINMENUCOMPONENT_H
