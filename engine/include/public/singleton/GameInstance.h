#ifndef GAMEINSTANCE_H
#define GAMEINSTANCE_H

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <framework/controller/PlayerController.h>
#include <singleton/Singleton.h>


namespace engine
{
    class GameInstance final : public Singleton<GameInstance>
    {
        friend class Singleton;

    public:
        template <typename controller_t, typename... controller_args_t>
            requires std::derived_from<controller_t, PlayerController> && std::constructible_from<controller_t, controller_args_t...>
        controller_t& add_player_controller( controller_args_t&&... args );
        void remove_player_controller( const PlayerController& controller );

        void set_gravity_coefficient( float coeffiecient );
        [[nodiscard]] float get_gravity_coefficient( ) const;

        void destroy( );

    private:
        std::vector<std::unique_ptr<PlayerController>> player_controllers_{};

        float gravity_coefficient_{ 10.f };

    };


    template <typename controller_t, typename ... controller_args_t> requires std::derived_from<controller_t, PlayerController> &&
        std::constructible_from<controller_t, controller_args_t...>
    controller_t& GameInstance::add_player_controller( controller_args_t&&... args )
    {
        auto& controller = player_controllers_.emplace_back( std::make_unique<controller_t>( std::forward<controller_args_t>( args )... ) );
        return static_cast<controller_t&>( *controller );
    }


    extern GameInstance& GAME_INSTANCE;

}


#endif //!GAMEINSTANCE_H
