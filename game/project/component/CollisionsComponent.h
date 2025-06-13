#ifndef COLLISIONSCOMPONENT_H
#define COLLISIONSCOMPONENT_H

#include <framework/component/Component.h>

#include <core/UID.h>
#include <framework/GameObject.h>
#include <framework/component/physics/ColliderComponent.h>


namespace engine
{
    class ColliderComponent;
    class PhysicsComponent;
}

namespace game
{
    class BubbleComponent;
    class BubbleCaptureComponent;
    class CollisionsComponent final : public engine::Component
    {
        using handlers_pair_t = std::pair<engine::UID, engine::UID>;
        using handler_fn_t    = std::function<void( engine::ColliderComponent&, engine::ColliderComponent&,
                                                    const engine::CollisionInfo& )>;

    public:
        explicit CollisionsComponent( engine::GameObject& owner );
        ~CollisionsComponent( ) noexcept override;

        CollisionsComponent( const CollisionsComponent& )                = delete;
        CollisionsComponent( CollisionsComponent&& ) noexcept            = delete;
        CollisionsComponent& operator=( const CollisionsComponent& )     = delete;
        CollisionsComponent& operator=( CollisionsComponent&& ) noexcept = delete;

    private:
        bool colliders_hooks_registered_{ false };
        std::map<handlers_pair_t, handler_fn_t> overlap_handlers_{};

        void owner_collider_begin_overlap( engine::ColliderComponent& self, engine::ColliderComponent& other,
                                           const engine::CollisionInfo& info );

        static void handle_default_overlap( engine::ColliderComponent& self, engine::ColliderComponent& other,
                                     const engine::CollisionInfo& info );

        static void handle_ally_bubble_overlap( engine::ColliderComponent& self, engine::ColliderComponent& other,
                                         const engine::CollisionInfo& info );
        static void handle_ally_fruit_overlap( const engine::ColliderComponent& self, const engine::ColliderComponent& other,
                                         const engine::CollisionInfo& info );
        static void handle_enemy_bubble_overlap( const engine::ColliderComponent& self, const engine::ColliderComponent& other,
                                          const engine::CollisionInfo& info );
        static void handle_ally_enemy_overlap( const engine::ColliderComponent& self, const engine::ColliderComponent& other,
                                        const engine::CollisionInfo& info );
        static void handle_bubble_bounce( const engine::ColliderComponent& self, engine::ColliderComponent& other,
                                        const engine::CollisionInfo& info );
        static void handle_ally_death( const engine::ColliderComponent& self, const engine::ColliderComponent& other,
                                         const engine::CollisionInfo& info );
        static void handle_bubble_destroy( const engine::ColliderComponent& self, engine::ColliderComponent& other,
                                        const engine::CollisionInfo& info );
        static void handle_rock_destroy( const engine::ColliderComponent& self, engine::ColliderComponent& other,
                                        const engine::CollisionInfo& info );
        static void handle_fruit_bounce( engine::ColliderComponent& self, engine::ColliderComponent& other,
                                        const engine::CollisionInfo& info );
        static void do_nothing( engine::ColliderComponent&, engine::ColliderComponent&,
                                         const engine::CollisionInfo& );

        template <typename component_t>
        static component_t* get_component( const engine::GameObject& object )
        {
            if ( const auto component = object.get_component<component_t>( );
                 component.has_value( ) )
            {
                return &component.value( );
            }
            return nullptr;
        }
        static float get_overlap_jitter( glm::vec2 normal );

    };

}


#endif //!COLLISIONSCOMPONENT_H
