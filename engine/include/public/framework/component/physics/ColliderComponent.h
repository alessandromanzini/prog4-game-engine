#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include <framework/component/Component.h>

#include <vec2.hpp>

#include <span>
#include <framework/event/MulticastDelegate.h>


namespace engine
{
    class ColliderComponent : public Component
    {
        Dispatcher<ColliderComponent&, ColliderComponent&> on_begin_overlap_dispatcher_{};
        Dispatcher<ColliderComponent&, ColliderComponent&> on_end_overlap_dispatcher_{};

    public:
        MulticastDelegate<ColliderComponent&, ColliderComponent&> on_begin_overlap{ on_begin_overlap_dispatcher_ };
        MulticastDelegate<ColliderComponent&, ColliderComponent&> on_end_overlap{ on_end_overlap_dispatcher_ };

        explicit ColliderComponent( owner_t& owner );

        [[nodiscard]] virtual bool hit_test( const glm::vec2& point ) const = 0;

        /**
         * Moves the collider by the given offset.
         * @param offset
         */
        void offset_by( const glm::vec2& offset );

        /**
         * Sets the offset of the collider to the given value.
         * @param offset
         */
        void offset_to( const glm::vec2& offset );

    protected:
        [[nodiscard]] virtual std::span<glm::vec2> get_pivots( ) const = 0;
        [[nodiscard]] bool hit_test( const ColliderComponent& ) const;

    private:
        glm::vec2 offset_{};


    };

}


#endif //!COLLIDERCOMPONENT_H
