#ifndef TYPE_UTILITY_HPP
#define TYPE_UTILITY_HPP

#include <string_view>

namespace type_utility
{
	template <typename class_t>
	consteval std::string_view type_name( )
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

	template <typename class_t>
	consteval std::string_view type_name( class_t&& )
	{
		return type_name<class_t>( );
	}

	template <typename class_t>
	consteval uint64_t type_hash( )
	{
		// Simple 64-bit FNV-1a hash
		constexpr uint64_t FNV_OFFSET_BASIS_64 = 0xCBF29CE484222325;
		constexpr uint64_t FNV_PRIME_64 = 0x100000001B3;

		uint64_t hash = FNV_OFFSET_BASIS_64;
		for ( char c : type_name<class_t>( ) )
		{
			hash ^= static_cast<uint64_t>( c );
			hash *= FNV_PRIME_64;
		}
		return hash;
	}

	template <typename class_t>
	consteval uint64_t type_hash( class_t&& )
	{
		return type_hash<class_t>( );
	}
}

#endif // !TYPE_UTILITY_HPP
