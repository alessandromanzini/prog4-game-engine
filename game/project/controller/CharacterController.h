#ifndef CHARACTERCONTROLLER_H
#define CHARACTERCONTROLLER_H

#include <framework/controller/PlayerController.h>


namespace game
{
    class GameOverComponent;
    class CharacterComponent;
    class JoinMenuComponent;
    class CharacterController final : public engine::PlayerController
    {
    public:
        CharacterController( JoinMenuComponent* joinComponent, engine::binding::DeviceType deviceType );
        void possess( engine::GameObject* pawn ) override;

        [[nodiscard]] bool has_joined( ) const;
        void set_block_selection( bool block );

        void set_gameover_component( GameOverComponent* gameover );

    private:
        CharacterComponent* state_ptr_{ nullptr };

        JoinMenuComponent* join_menu_component_ptr_{ nullptr };
        GameOverComponent* game_over_component_ptr_{ nullptr };

        engine::binding::DeviceInfo device_info_{};
        bool has_joined_{ false };
        bool block_selection_{ false };

        void device_registered( engine::InputMappingContext& context, engine::binding::DeviceInfo deviceInfo ) override;

        void move( float dir ) const;
        void jump( ) const;
        void attack( ) const;

        void join( );
        void leave( );
        void menu_select( float direction ) const;
        void confirm( ) const;
        void name_select( glm::vec2 selection ) const;

    };

}


#endif //!CHARACTERCONTROLLER_H
