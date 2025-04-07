#ifndef BINDINGTRAITS_H
#define BINDINGTRAITS_H


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

    // Const member
    template <typename _class_t, typename _param_t>
    struct method_traits<void ( _class_t::* )( _param_t ) const>
    {
        using class_t = _class_t;
        using param_t = _param_t;
    };


    // +---------------------------+
    // | FUNCTION TRAITS           |
    // +---------------------------+
    template <typename function_sig_t>
    struct function_traits;

    template <typename _return_t, typename _param_t>
    struct function_traits<std::function<_return_t(_param_t)>>
    {
        using return_t = _return_t;
        using param_t = _param_t;
    };


	// +---------------------------+
	// | BAD CONVERSION            |
	// +---------------------------+
	template <typename from_t, typename to_t>
	constexpr bool bad_conversion = false;

}


#endif //!BINDINGTRAITS_H
