#ifndef SDLSOUNDSYSTEM_H
#define SDLSOUNDSYSTEM_H

#include <services/sound/SoundSystem.h>

#include <cstdint>


namespace engine
{
class SdlSoundSystem : public SoundSystem
{
public:
    SdlSoundSystem( )  = default;
    ~SdlSoundSystem( ) = default;

    SdlSoundSystem( const SdlSoundSystem& )                = delete;
    SdlSoundSystem( SdlSoundSystem&& ) noexcept            = delete;
    SdlSoundSystem& operator=( const SdlSoundSystem& )     = delete;
    SdlSoundSystem& operator=( SdlSoundSystem&& ) noexcept = delete;
    
private:
    static constexpr uint8_t MAX_CHANNELS_{ 16 };
    static constexpr uint8_t MAX_PENDING_{ 32 };

};

}


#endif //!SDLSOUNDSYSTEM_H
