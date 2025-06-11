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

        QUIT
    };


    void initialize_input( );
}


#endif //!INPUTUTILS_H
