#ifndef BINDINGTRAITS_H
#define BINDINGTRAITS_H

#include <functional>


namespace engine::binding
{
    // +---------------------------+
    // | METHOD TRAITS             |
    // +---------------------------+
    template <typename method_sig_t>
    struct method_traits;

    // Non-const member
    template <typename _class_t, typename _param_t>
    struct method_traits<void ( _class_t::* )( _param_t )>
    {
        using class_t = _class_t;
        using param_t = _param_t;
    };

    // Non-const parameterless member
    template <typename _class_t>
    struct method_traits<void ( _class_t::* )( )>
    {
        using class_t = _class_t;
        using param_t = void;
    };

    // Const member
    template <typename _class_t, typename _param_t>
    struct method_traits<void ( _class_t::* )( _param_t ) const>
    {
        using class_t = _class_t;
        using param_t = _param_t;
    };

    // Const parameterless member
    template <typename _class_t>
    struct method_traits<void ( _class_t::* )( ) const>
    {
        using class_t = _class_t;
        using param_t = void;
    };

    // +---------------------------+
    // | FUNCTION TRAITS           |
    // +---------------------------+
    template <typename function_sig_t>
    struct function_traits;

    template <typename _return_t, typename... _params_t>
    struct function_traits<std::function<_return_t(_params_t...)>>
    {
        using return_t = _return_t;
        using params_t = std::tuple<_params_t...>;
    };

    template <typename _return_t, typename... _params_t>
    struct function_traits<_return_t(_params_t...)>
    {
        using return_t = _return_t;
        using params_t = std::tuple<_params_t...>;
    };


	// +---------------------------+
	// | BAD CONVERSION            |
	// +---------------------------+
	template <typename from_t, typename to_t>
	constexpr bool BAD_CONVERSION = false;

}


#endif //!BINDINGTRAITS_H
