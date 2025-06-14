#ifndef INPUTUTILS_H
#define INPUTUTILS_H


namespace game
{
    enum class IA
    {
        MOVE,
        JUMP,
        ATTACK,

        JOIN,
        LEAVE,
        MODE_SELECTION,
        CONFIRM,

        SKIP_LEVEL,
        TOGGLE_MUTE,
        QUIT
    };


    void initialize_input( );
}


#endif //!INPUTUTILS_H
