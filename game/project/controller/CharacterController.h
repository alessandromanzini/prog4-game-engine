#ifndef CHARACTERCONTROLLER_H
#define CHARACTERCONTROLLER_H

#include <framework/controller/PlayerController.h>


namespace game
{
    class CharacterComponent;
    class CharacterController final : public engine::PlayerController
    {
    public:
        CharacterController( );
        void possess( engine::GameObject* pawn ) override;

    private:
        CharacterComponent* state_ptr_{ nullptr };

        void device_registered( engine::InputMappingContext& context, engine::binding::DeviceInfo deviceInfo ) override;

        void move( glm::vec2 dir ) const;
        void jump( ) const;
        void attack( ) const;

    };

}


#endif //!CHARACTERCONTROLLER_H
