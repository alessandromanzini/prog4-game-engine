#ifndef INPUTUTILS_H
#define INPUTUTILS_H


namespace game
{
    enum class IA
    {
        MOVE,
        PLAY_SOUND,
        KILL,

        VOLUME_UP,
        VOLUME_DOWN,

        QUIT
    };


    void initialize_input( );
}


#endif //!INPUTUTILS_H
