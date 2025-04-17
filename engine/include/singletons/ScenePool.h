#ifndef SCENEPOOL_H
#define SCENEPOOL_H

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <framework/Scene.h>
#include <singletons/Singleton.h>

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <memory>
#include <vector>


namespace engine
{
    class ScenePool final : public Singleton<ScenePool>
    {
        friend class Singleton;
    public:
        ~ScenePool( ) override = default;

        ScenePool( const ScenePool& )                = delete;
        ScenePool( ScenePool&& ) noexcept            = delete;
        ScenePool& operator=( const ScenePool& )     = delete;
        ScenePool& operator=( ScenePool&& ) noexcept = delete;

        Scene& create_scene( const std::string& name );

        void fixed_update( );
        void update( );
        void render( );

        [[nodiscard]] Scene& get_active_scene( ) const;
        [[nodiscard]] Scene& get_scene( const std::string& name ) const;
        [[nodiscard]] Scene& get_scene( uint16_t id ) const;

        void cleanup( ) const;

    private:
        std::vector<std::unique_ptr<Scene>> scenes_;
        Scene* active_scene_ptr_{ nullptr };

        ScenePool( ) = default;

    };

    extern ScenePool& SCENE_POOL;
}

#endif //!SCENEPOOL_H
