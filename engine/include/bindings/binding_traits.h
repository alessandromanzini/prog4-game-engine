#ifndef BINDINGTRAITS_H
#define BINDINGTRAITS_H


namespace engine::binding
{
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

}


#endif //!BINDINGTRAITS_H
