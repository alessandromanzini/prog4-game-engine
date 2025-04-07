#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <controllers/BaseController.h>

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <bindings/InputMappingContext.h>


namespace engine
{
    class PlayerController : public BaseController
    {
    public:
        ~PlayerController( ) noexcept override;

        [[nodiscard]] bool has_registered_device( ) const;

    protected:
        PlayerController( ) = default;

        void try_register_device( binding::DeviceType deviceType,
                                  std::chrono::milliseconds timeout = std::chrono::milliseconds( 2000 ) );

        /**
         * This method is called when a device is registered.
         * @param context The input mapping context that registered the device
         * @param deviceInfo Info of the newly registered device
         */
        virtual void device_registered( InputMappingContext& context, binding::DeviceInfo deviceInfo );

    private:
        static constexpr std::chrono::milliseconds REGISTER_ATTEMPT_TIME_STEP_{ 100 };

        bool has_registered_device_{ false };

        void try_register_keyboard_impl( InputMappingContext& context, std::chrono::milliseconds timeout );
        void try_register_gamepad_impl( InputMappingContext& context, std::chrono::milliseconds timeout );

    };

}


#endif //!PLAYERCONTROLLER_H
