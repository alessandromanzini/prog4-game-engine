#ifndef RIGIDBODYCOMPONENT_H
#define RIGIDBODYCOMPONENT_H

#include <framework/component/Component.h>

#include <glm.hpp>


namespace engine
{
    class RigidBodyComponent final : public Component
    {
    public:
        explicit RigidBodyComponent( owner_t& owner );

        void set_simulate_physics( bool simulate );
        [[nodiscard]] bool is_simulating_physics( ) const;

        [[nodiscard]] glm::vec2 get_velocity( ) const;

        void add_force( glm::vec2 force, bool acceleration = false );

        void tick( ) override;

    private:
        bool simulate_physics_{ true };

        glm::vec2 velocity_{};

    };

}


#endif //!RIGIDBODYCOMPONENT_H
