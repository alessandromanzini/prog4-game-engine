#ifndef UID_H
#define UID_H

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include "core/type_utility.hpp"

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <functional>


namespace engine
{
    // ReSharper disable once CppInconsistentNaming
    struct UID final
    {
        template<typename value_t>
        explicit constexpr UID( value_t value )
            : uid{ type_utility::hash_cast( static_cast<type_utility::size_hash_t>( value ) ) } { }


        explicit constexpr UID( const char* cstr )
            : uid{ type_utility::hash_cast( std::string_view( cstr ) ) } { }


        explicit operator type_utility::hash_value_t( ) const
        {
            return uid;
        }


        bool operator==( const UID& other ) const
        {
            return uid == other.uid;
        }


        type_utility::hash_value_t uid{};

    };

}

// Specialized hash function for UID
template<>
struct std::hash<engine::UID>
{
    size_t operator()( const engine::UID& uid ) const noexcept
    {
        return std::hash<type_utility::hash_value_t>( )( uid.uid );
    }
};

#endif //!UID_H
