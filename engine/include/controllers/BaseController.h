#ifndef BASECONTROLLER_H
#define BASECONTROLLER_H

#include <framework/GameObject.h>


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

        /**
         * This method is called when a controller possesses a GameObject.
         * @param pawn The pointer to the possessed GameObject
         */
        virtual void possess( GameObject* pawn ) { pawn_ptr_ = pawn; }

        /**
         * This method is called when a controller unpossesses a GameObject.
         */
        virtual void unpossess( ) { pawn_ptr_ = nullptr; }

        [[nodiscard]] const GameObject* get_pawn( ) const { return pawn_ptr_; }
        [[nodiscard]] GameObject* get_pawn( ) { return pawn_ptr_; }

        [[nodiscard]] int32_t get_id( ) const { return id_; }

        [[nodiscard]] bool operator==( const BaseController& other ) const { return id_ == other.id_; }

    protected:
        const int32_t id_{ -1 };


        BaseController( )
            : id_( s_id_counter_++ ) { }

    private:
        inline static int32_t s_id_counter_{ 0 };

        GameObject* pawn_ptr_{ nullptr };

    };

}


#endif //!BASECONTROLLER_H
