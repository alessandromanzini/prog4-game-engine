#ifndef FRUITCOMPONENT_H
#define FRUITCOMPONENT_H

#include <framework/component/Component.h>

#include <vec2.hpp>


namespace game
{
    class FruitComponent final : public engine::Component
    {
    public:
        explicit FruitComponent( owner_t& owner, int value, int bounces, glm::vec2 preferredDirection = {} );
        void tick( ) override;
        void bounce( glm::vec2 normal, float depth );

        [[nodiscard]] int get_fruit_value( ) const;
        [[nodiscard]] bool is_capturable( ) const;

    private:
        static constexpr float SPEED_{ 150.f };
        static constexpr float IFRAMES_DURATION_{ .25f };

        const int value_{};
        int bounces_{};
        glm::vec2 direction_{ 0.f, 0.f };

        float iframes_{ 0.f };

        bool is_moving( ) const;

    };

}


#endif //!FRUITCOMPONENT_H
