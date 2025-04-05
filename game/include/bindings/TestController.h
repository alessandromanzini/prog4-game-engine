#ifndef TESTCONTROLLER_H
#define TESTCONTROLLER_H

#include <controllers/PlayerController.h>
#include <singletons/InputSystem.h>

#include <iostream>


namespace game
{
    class TestController final : public engine::PlayerController
    {
    public:
        TestController( ) : PlayerController( 0 )
        {
            auto& imc = engine::INPUT_SYSTEM.get_input_mapping_context( );

            const auto id = engine::INPUT_SYSTEM.fetch_free_gamepad_id( );
            imc.register_device( *this, { engine::binding::DeviceType::GAMEPAD, id } );

            imc.bind_to_input_action( *this, engine::UID( IA::TEST ), &TestController::testFunc, engine::binding::TriggerEvent::TRIGGERED );
            imc.bind_to_input_action( *this, engine::UID( IA::TEST ), &TestController::testFunc, engine::binding::TriggerEvent::RELEASED );
        }

        ~TestController( ) override
        {
            auto& imc = engine::INPUT_SYSTEM.get_input_mapping_context( );

            imc.unregister_device( *this );
        }

        void testFunc( const bool val ) const
        {
            std::cout << "Test function called! " << std::boolalpha << val << std::endl;
        }

    };

}


#endif //!TESTCONTROLLER_H
