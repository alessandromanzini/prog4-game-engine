#ifndef SCENEPOOL_H
#define SCENEPOOL_H

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include "Scene.h"
#include "Singleton.h"

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <memory>
#include <string>
#include <vector>


namespace engine
{
    class ScenePool final : public Singleton<ScenePool>
    {
    public:
        Scene& create_scene( const std::string& name );

        void fixed_update( );
        void update( );
        void render( );

        [[nodiscard]] Scene& get_active_scene( ) const;
        [[nodiscard]] Scene& get_scene( const std::string& name ) const;
        [[nodiscard]] Scene& get_scene( uint16_t id ) const;

        void cleanup( ) const;

        friend class Singleton<ScenePool>;

    private:
        std::vector<std::unique_ptr<Scene>> scenes_;
        Scene* active_scene_ptr_{ nullptr };

        ScenePool( ) = default;

    };

    extern ScenePool& SCENE_POOL;
}

#endif //!SCENEPOOL_H
