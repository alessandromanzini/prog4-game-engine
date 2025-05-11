#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <core/type_utility.hpp>
#include <framework/Deletable.h>
#include <framework/GameObject.h>
#include <framework/Transform.h>
#include <framework/components/Component.h>
#include <framework/event/MulticastDelegate.h>
#include <framework/resources/data/Deleter.h>

#include <core/meta/concepts.h>

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <map>
#include <stdexcept>


namespace engine
{
    class Scene;
    class Component;
    class GameObjectView;
    class GameObject final : public Deletable
    {
        MulticastDelegate<void> on_deletion_{};

    public:
        explicit GameObject( Scene* scene );
        ~GameObject( ) noexcept override;

        GameObject( const GameObject& other )                = delete;
        GameObject( GameObject&& other ) noexcept            = delete;
        GameObject& operator=( const GameObject& other )     = delete;
        GameObject& operator=( GameObject&& other ) noexcept = delete;

        void fixed_tick( );
        void tick( );
        void render( ) const;

        void cleanup( );

        void set_parent( GameObject* parent, bool keepWorldPosition = true );

        [[nodiscard]] const Transform& get_world_transform( );
        [[nodiscard]] const Transform& get_local_transform( ) const;
        void set_world_transform( const Transform& transform );
        void set_local_transform( const Transform& transform );
        void set_local_transform( Transform&& transform );

        template <typename component_t, typename... args_t>
            requires meta::DerivedComponentWithBaseContructor<component_t, args_t...>
        component_t& add_component( args_t&&... args );

        template <typename component_t>
            requires meta::DerivedComponent<component_t>
        [[nodiscard]] component_t& get_component( ) const;

        template <typename component_t>
            requires meta::DerivedComponent<component_t>
        [[nodiscard]] component_t& get_components( ) const;

        void remove_component( Component& component );

        [[nodiscard]] std::vector<GameObject*>& get_children( );
        [[nodiscard]] const std::vector<GameObject*>& get_children( ) const;
        void collect_children( std::vector<GameObject*>& children ) const;

        void bind_to_object_deletion( decltype(on_deletion_)::delegate_t&& delegate );
        void unbind_from_object_deletion( decltype(on_deletion_)::delegate_t&& delegate );

        void mark_for_deletion( ) override;

    private:
        // std::unique_ptr<GameObjectView> view_ptr_{};
        Scene* scene_ptr_{ nullptr };
        GameObject* parent_ptr_{ nullptr };

        Transform local_transform_{};
        Transform world_transform_{};
        bool transform_dirty_{ false };

        std::multimap<uint64_t, std::unique_ptr<Component>> components_{};
        Deleter<Component> deleter_{};

        std::vector<GameObject*> children_{};

        [[nodiscard]] bool is_child( const GameObject* gameObject ) const;
        void add_child( GameObject* gameObject );
        void remove_child( GameObject* gameObject );
        [[nodiscard]] bool has_children( ) const;

        void set_transform_dirty( );

        void update_world_transform( );

    };


    template <typename component_t, typename... args_t>
        requires meta::DerivedComponentWithBaseContructor<component_t, args_t...>
    component_t& GameObject::add_component( args_t&&... args )
    {
        // Initializing component with GameObjectView and arguments' perfect forwarding.
        // We insert the element in the multimap with its compile-time type hash as the key.
        auto component = std::make_unique<component_t>( *this, std::forward<args_t>( args )... );
        auto it        = components_.insert(
            std::make_pair( type_utility::type_hash<component_t>( ), std::move( component ) ) );
        return static_cast<component_t&>( *( it->second ) );
    }


    template <typename component_t>
        requires meta::DerivedComponent<component_t>
    [[nodiscard]] component_t& GameObject::get_component( ) const
    {
        auto it = components_.find( type_utility::type_hash<component_t>( ) );
        assert( it != components_.end( ) && "Component not found!" );
        return static_cast<component_t&>( *it->second.get( ) );
    }


    template <typename component_t>
        requires meta::DerivedComponent<component_t>
    [[nodiscard]] component_t& GameObject::get_components( ) const
    {
        throw std::runtime_error( "Not implemented!" );
    }

}

#endif // GAMEOBJECT_H
