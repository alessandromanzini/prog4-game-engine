#ifndef RIGIDBODYCOMPONENT_H
#define RIGIDBODYCOMPONENT_H

#include <framework/components/Component.h>

#include <glm.hpp>


namespace engine
{
    enum class FaceDirection
    {
        LEFT,
        RIGHT
    };

    class RigidBodyComponent final : public Component
    {
    public:
        explicit RigidBodyComponent( owner_t& owner );

        [[nodiscard]] glm::vec2 get_velocity( ) const;

        void add_velocity( glm::vec2 velocity );

        // Temp function
        void reset_horizontal_velocity( );

        void tick( ) override;

        [[nodiscard]] FaceDirection get_face_direction( ) const;

    private:
        glm::vec2 velocity_{};

        FaceDirection face_direction_{};

    };

}


#endif //!RIGIDBODYCOMPONENT_H
