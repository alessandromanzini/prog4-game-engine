#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include <framework/component/Component.h>
#include <framework/event/MulticastDelegate.h>
#include <framework/resource/data/SparseSet.h>

#include <vec2.hpp>

#include <span>


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
        ~ColliderComponent( ) override;

        ColliderComponent( const ColliderComponent& )                = delete;
        ColliderComponent( ColliderComponent&& ) noexcept            = delete;
        ColliderComponent& operator=( const ColliderComponent& )     = delete;
        ColliderComponent& operator=( ColliderComponent&& ) noexcept = delete;

        void tick( ) override;
        static void late_tick( );

        [[nodiscard]] virtual bool hit_test( glm::vec2 ) const = 0;
        bool hit_test( ColliderComponent& other, bool ignoreOverlaps = false );

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

        [[nodiscard]] glm::vec2 get_offset( ) const;

    protected:
        [[nodiscard]] virtual std::span<const glm::vec2> get_pivots( ) const = 0;

    private:
        inline static std::vector<ColliderComponent*> s_colliders_{};
        inline static SparseSet<std::pair<ColliderComponent*, ColliderComponent*>> s_colliders_cross_check_{};

        glm::vec2 offset_{};

        SparseSet<ColliderComponent*> overlapping_colliders_{};

        [[nodiscard]] bool hit_test_impl( const ColliderComponent& other ) const;

        void handle_begin_overlap( ColliderComponent& other );
        void handle_end_overlap( ColliderComponent& other );

        [[nodiscard]] std::pair<ColliderComponent*, ColliderComponent*> make_cross_check_pair( ColliderComponent& other );

    };

}


#endif //!COLLIDERCOMPONENT_H
