#ifndef INPUTUTILS_H
#define INPUTUTILS_H


namespace game
{
    // TODO: I still need to create controllers to separate the input actions
    // for each player
    enum class IA
    {
        KATTACK,
        GATTACK,

        KSCORE_UP,
        GSCORE_UP,

        KSCORE_GREAT_UP,
        GSCORE_GREAT_UP
    };

    void initialize_input( );
}


#endif // !INPUTUTILS_H
