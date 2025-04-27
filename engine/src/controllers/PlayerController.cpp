#include <controllers/PlayerController.h>

// +---------------------------+
// | PROJECT HEADERS           |
// +---------------------------+
#include <singletons/InputSystem.h>

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <thread>

using namespace engine::binding;


namespace engine
{
    PlayerController::~PlayerController( ) noexcept
    {
        if ( has_registered_device_.clone( ) )
        {
            auto& context = INPUT_SYSTEM.get_input_mapping_context( );
            context.unregister_device( *this );
            has_registered_device_.set( false );
        }
    }


    bool PlayerController::has_registered_device( ) const
    {
        return has_registered_device_.clone( );
    }


    void PlayerController::try_register_device( const DeviceType deviceType, const std::chrono::milliseconds timeout )
    {
        auto& context = INPUT_SYSTEM.get_input_mapping_context( );
        switch ( deviceType )
        {
            case DeviceType::KEYBOARD:
                try_register_keyboard_impl( context, timeout );
                break;

            case DeviceType::GAMEPAD:
                // Detach the thread to allow it to run independently
                std::thread( &PlayerController::try_register_gamepad_impl, this, std::ref( context ),
                             timeout ).detach( );
                break;
        }
    }


    void PlayerController::device_registered( InputMappingContext& /* context */, DeviceInfo /* deviceInfo */ ) { }


    void PlayerController::try_register_keyboard_impl( InputMappingContext& context,
                                                       std::chrono::milliseconds /* timeout */ )
    {
        constexpr DeviceInfo deviceInfo{ DeviceType::KEYBOARD };

        context.register_device( *this, deviceInfo );
        has_registered_device_.set( true );

        device_registered( context, deviceInfo );
    }


    void PlayerController::try_register_gamepad_impl( InputMappingContext& context, std::chrono::milliseconds timeout )
    {
        while ( timeout > std::chrono::milliseconds( 0 ) )
        {
            try
            {
                const DeviceInfo deviceInfo{ DeviceType::GAMEPAD, INPUT_SYSTEM.fetch_free_gamepad_id( ) };

                context.register_device( *this, deviceInfo );

                has_registered_device_.set( true );

                device_registered( context, deviceInfo );

                break;
            }
            catch ( std::runtime_error& )
            {
                has_registered_device_.set( false );
            }

            std::this_thread::sleep_for( REGISTER_ATTEMPT_TIME_STEP_ );
            timeout -= REGISTER_ATTEMPT_TIME_STEP_;
        }
    }

}
