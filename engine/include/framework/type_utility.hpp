#ifndef TYPE_UTILITY_HPP
#define TYPE_UTILITY_HPP

#include <string_view>

namespace type_utility
{
	// +--------------------------------+
	// | HASH TYPES						|
	// +--------------------------------+
	typedef uint64_t size_hash_t;
	typedef uint64_t hash_value_t;

	// +--------------------------------+
	// | TYPE NAME EVALUATION			|
	// +--------------------------------+
	template <typename class_t>
	consteval [[nodiscard]] std::string_view type_name( )
	{
#if defined(__clang__) || defined(__GNUC__)
		constexpr std::string_view function = __PRETTY_FUNCTION__;
		constexpr std::string_view prefix = "type_utility::type_name() [class_t = ";
		constexpr std::string_view suffix = "]";
#elif defined(_MSC_VER)
		constexpr std::string_view function = __FUNCSIG__;
		constexpr std::string_view prefix = "type_utility::type_name<class ";
		//constexpr std::string_view prefix = "std::string_view __cdecl type_name<";
		constexpr std::string_view suffix = ">";
#else
#error "Compiler not supported"
#endif
		constexpr std::size_t prefixPos = function.find( prefix );
		static_assert( prefixPos != std::string_view::npos, "prefix format is incorrect!" );

		constexpr std::size_t start = prefixPos + prefix.size( );
		constexpr std::size_t end = function.rfind( suffix );
		return function.substr( start, end - start );
	}

	// +--------------------------------+
	// | COMPILE TIME HASH CASTING		|
	// +--------------------------------+
	constexpr [[nodiscard]] hash_value_t hash_cast( size_hash_t value )
	{
		constexpr hash_value_t FNV_OFFSET_BASIS_64 = 0xCBF29CE484222325;
		constexpr hash_value_t FNV_PRIME_64 = 0x100000001B3;

		hash_value_t hash = FNV_OFFSET_BASIS_64;

		// Process each byte of the integer
		for ( size_t i = 0; i < sizeof( value ); ++i )
		{
			hash ^= ( value >> ( i * 8 ) ) & 0xFF; // Extract byte
			hash *= FNV_PRIME_64;
		}

		return hash;
	}

	constexpr [[nodiscard]] hash_value_t hash_cast( const std::string_view& view )
	{
		// Simple 64-bit FNV-1a hash
		constexpr hash_value_t FNV_OFFSET_BASIS_64 = 0xCBF29CE484222325;
		constexpr hash_value_t FNV_PRIME_64 = 0x100000001B3;

		hash_value_t hash = FNV_OFFSET_BASIS_64;
		for ( char c : view )
		{
			hash ^= static_cast<hash_value_t>( c );
			hash *= FNV_PRIME_64;
		}
		return hash;
	}

	// +--------------------------------+
	// | TYPE HASHING UTILITY			|
	// +--------------------------------+
	template <typename class_t>
	consteval [[nodiscard]] std::string_view type_name( class_t&& )
	{
		return type_name<class_t>( );
	}

	template <typename class_t>
	consteval [[nodiscard]] hash_value_t type_hash( )
	{
		return hash_cast( type_name<class_t>( ) );
	}

	template <typename class_t>
	consteval [[nodiscard]] hash_value_t type_hash( class_t&& )
	{
		return type_hash<class_t>( );
	}

}

#endif // !TYPE_UTILITY_HPP
