#ifndef OBSERVER_H
#define OBSERVER_H

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <Subject.h>
#include <framework/UID.h>


namespace engine
{
    class Observer
    {
    public:
        Observer( )          = default;
        virtual ~Observer( ) = default;

        Observer( const Observer& )                = delete;
        Observer( Observer&& ) noexcept            = delete;
        Observer& operator=( const Observer& )     = delete;
        Observer& operator=( Observer&& ) noexcept = delete;

        virtual void notify( UID event, Subject* subject, event::broadcast_value_variant_t value ) = 0;

    private:
        friend class Subject;

        Observer* next_ptr_{ nullptr };

    };

}

#endif //!OBSERVER_H
