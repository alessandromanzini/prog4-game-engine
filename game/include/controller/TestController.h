#ifndef TESTCONTROLLER_H
#define TESTCONTROLLER_H

#include <framework/component/AudioComponent.h>
#include <framework/controller/PlayerController.h>

namespace engine
{
    class RigidBodyComponent;
}

namespace game
{
    class TestController final : public engine::PlayerController
    {
    public:
        TestController( );

        void possess( engine::GameObject* pawn ) override;

    private:
        engine::AudioComponent* audio_ptr_{ nullptr };
        engine::RigidBodyComponent* rigid_body_ptr_{ nullptr };

        void device_registered( engine::InputMappingContext& context, engine::binding::DeviceInfo deviceInfo ) override;

        void move( glm::vec2 dir ) const;
        void jump( ) const;

        void play_sound( ) const;
        void volume_up( ) const;
        void volume_down( ) const;

        void kill( );

        void quit( ) const;

    };

}


#endif //!TESTCONTROLLER_H
