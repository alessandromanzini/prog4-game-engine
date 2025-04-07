#ifndef TESTCONTROLLER_H
#define TESTCONTROLLER_H

#include <controllers/PlayerController.h>

#include <iostream>


namespace game
{
    class TestController final : public engine::PlayerController
    {
    public:
        TestController( )
        {
            try_register_device( engine::binding::DeviceType::KEYBOARD );
        }


        void testFunc( const glm::vec2 val ) const
        {
            if ( not( val.x == 0 && val.y == 0 ) )
            {
                get_pawn( )->get_component<MoveComponent>( ).move( val );
            }
        }

    protected:
        void device_registered( engine::InputMappingContext& context, const engine::binding::DeviceInfo deviceInfo ) override
        {
            std::cout << "Test device registered at id " << static_cast<int>( deviceInfo.id ) << std::endl;

            context.bind_to_input_action( *this, engine::UID( IA::TEST ), &TestController::testFunc,
                                          engine::binding::TriggerEvent::PRESSED );
            //context.bind_to_input_action( *this, engine::UID( IA::TEST ), &TestController::testFunc, engine::binding::TriggerEvent::RELEASED );
        }

    };

}


#endif //!TESTCONTROLLER_H
