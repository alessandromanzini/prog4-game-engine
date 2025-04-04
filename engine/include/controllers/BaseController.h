#ifndef BASECONTROLLER_H
#define BASECONTROLLER_H

#include <GameObject.h>


namespace engine
{
    class BaseController
    {
    public:
        virtual ~BaseController( ) = default;

        virtual void possess( GameObject* pawn ) noexcept { pawn_ptr_ = pawn; }
        virtual void unpossess( ) noexcept { pawn_ptr_ = nullptr; }

        const GameObject* get_pawn( ) const noexcept { return pawn_ptr_; }
        GameObject* get_pawn( ) noexcept { return pawn_ptr_; }

    protected:
        explicit BaseController( ) = default;

    private:
        GameObject* pawn_ptr_{ nullptr };

    };

}


#endif //!BASECONTROLLER_H
