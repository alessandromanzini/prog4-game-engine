#ifndef BASECONTROLLER_H
#define BASECONTROLLER_H

#include <GameObject.h>


namespace engine
{
    class BaseController
    {
    public:
        virtual ~BaseController( ) = default;

        BaseController( const BaseController& )                = delete;
        BaseController( BaseController&& ) noexcept            = delete;
        BaseController& operator=( const BaseController& )     = delete;
        BaseController& operator=( BaseController&& ) noexcept = delete;

        virtual void possess( GameObject* pawn ) { pawn_ptr_ = pawn; }
        virtual void unpossess( ) { pawn_ptr_ = nullptr; }

        [[nodiscard]] const GameObject* get_pawn( ) const { return pawn_ptr_; }
        [[nodiscard]] GameObject* get_pawn( ) { return pawn_ptr_; }

    protected:
        BaseController( ) = default;

    private:
        GameObject* pawn_ptr_{ nullptr };

    };

}


#endif //!BASECONTROLLER_H
