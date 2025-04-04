#ifndef SCENE_H
#define SCENE_H

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include "framework/Deleter.h"
#include "framework/GameObject.h"


namespace engine
{
    class Scene final
    {
    public:
        explicit Scene( const std::string& name );
        ~Scene( ) noexcept = default;

        Scene( const Scene& )                = delete;
        Scene( Scene&& ) noexcept            = delete;
        Scene& operator=( const Scene& )     = delete;
        Scene& operator=( Scene&& ) noexcept = delete;

        void add( std::unique_ptr<GameObject> object );
        void remove( GameObject* object );
        void remove_all( );

        GameObject* create_object( );

        [[nodiscard]] const std::string& get_name( ) const;
        [[nodiscard]] uint16_t get_id( ) const;

        void fixed_update( );
        void update( );
        void render( ) const;

        void cleanup( );

    private:
        static uint16_t s_id_counter_;

        std::string name_;
        uint16_t id_{};

        std::vector<std::unique_ptr<GameObject>> objects_{};
        Deleter<GameObject> deleter_{};

    };

}

#endif //!SCENE_H
