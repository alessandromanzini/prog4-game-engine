#ifndef ROCKCOMPONENT_H
#define ROCKCOMPONENT_H

#include <framework/component/Component.h>
#include <framework/resource/texture/Sprite2D.h>

#include <vec2.hpp>


namespace game
{
    class RockComponent final : public engine::Component
    {
    public:
        explicit RockComponent( owner_t& owner, glm::vec2 initialDirection, float speed );
        void tick( ) override;
        void render( ) const override;

        void destroy( );

    private:
        const float rock_speed_{};
        glm::vec2 direction_{};

        engine::Sprite2D default_sprite_;
        engine::Sprite2D destory_sprite_;
        engine::Sprite2D* current_sprite_ptr_{ &default_sprite_ };

        void manage_lifetime( ) const;

    };

}


#endif //!ROCKCOMPONENT_H
