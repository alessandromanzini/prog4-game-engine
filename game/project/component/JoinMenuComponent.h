#ifndef JOINMENUCOMPONENT_H
#define JOINMENUCOMPONENT_H

#include <framework/component/Component.h>

#include <framework/binding/binding_device.h>

#include <vector>


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

        void join( engine::binding::DeviceInfo device );
        void leave( uint8_t id );

    private:
        engine::GameObject* selection_object_ptr_{ nullptr };
        std::vector<engine::GameObject*> player_objects_{};

        std::array<std::optional<engine::binding::DeviceInfo>, 2> slot_devices_{};

        void arrange_ui( engine::GameObject* object, const engine::binding::DeviceInfo& info ) const;

    };

}


#endif //!JOINMENUCOMPONENT_H
