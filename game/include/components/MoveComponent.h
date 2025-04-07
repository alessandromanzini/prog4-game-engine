#ifndef MOVECOMPONENT_H
#define MOVECOMPONENT_H

#include <components/Component.h>


namespace game
{
    class MoveComponent final : public engine::Component
    {
    public:
        explicit MoveComponent( owner_t& owner, float speed );
        ~MoveComponent( ) override = default;

        MoveComponent( const MoveComponent& )                = delete;
        MoveComponent( MoveComponent&& ) noexcept            = delete;
        MoveComponent& operator=( const MoveComponent& )     = delete;
        MoveComponent& operator=( MoveComponent&& ) noexcept = delete;

        void move( glm::vec2 direction ) const;

    private:
        const float speed_;

    };

}


#endif //!MOVECOMPONENT_H
