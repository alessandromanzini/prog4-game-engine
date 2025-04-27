#ifndef TESTCONTROLLER_H
#define TESTCONTROLLER_H

#include <controllers/PlayerController.h>
#include <framework/components/AudioComponent.h>
#include <components/MoveComponent.h>

namespace game
{
    class TestController final : public engine::PlayerController
    {
    public:
        TestController( );

        void possess( engine::GameObject* pawn ) override;

    private:
        engine::AudioComponent* audio_ptr_{ nullptr };
        MoveComponent* move_ptr_{ nullptr };

        void device_registered( engine::InputMappingContext& context, const engine::binding::DeviceInfo deviceInfo ) override;

        void move( glm::vec2 dir ) const;

        void play_sound( ) const;
        void volume_up( ) const;
        void volume_down( ) const;

        void kill( );

        void quit( ) const;

    };

}


#endif //!TESTCONTROLLER_H
