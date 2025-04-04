#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <controllers/BaseController.h>


namespace engine
{
    class PlayerController : public BaseController
    {
    public:
        bool operator==( const PlayerController& other ) const noexcept
        {
            return id_ == other.id_;
        }

    protected:
        const int id_{ -1 };

        explicit PlayerController( const int id ) noexcept
            : id_( id )
        {
            assert( id_ >= 0 && "PlayerController ID must be non-negative!" );
        }

    };

}


#endif //!PLAYERCONTROLLER_H
