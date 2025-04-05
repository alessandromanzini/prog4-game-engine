#ifndef SUBJECT_H
#define SUBJECT_H

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <framework/UID.h>

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <variant>


namespace engine
{
    namespace event
    {
        using broadcast_value_variant_t = std::variant<int, float, double, std::string_view>;
    }

    class Observer;
    class Subject final
    {
    public:
        Subject( )           = default;
        ~Subject( ) noexcept = default;

        Subject( const Subject& )                = delete;
        Subject( Subject&& ) noexcept            = delete;
        Subject& operator=( const Subject& )     = delete;
        Subject& operator=( Subject&& ) noexcept = delete;

        void add_observer( Observer& observer );
        void remove_observer( const Observer& observer );

        void broadcast( UID event, const event::broadcast_value_variant_t& value = {} );

    private:
        Observer* head_ptr_{ nullptr };

    };

}

#endif //!SUBJECT_H
