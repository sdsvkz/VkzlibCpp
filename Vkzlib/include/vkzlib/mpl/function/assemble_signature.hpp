#ifndef VKZLIB_MPL_FUNCTION_ASSEMBLE_SIGNATURE_HPP
#define VKZLIB_MPL_FUNCTION_ASSEMBLE_SIGNATURE_HPP

namespace vkz::mpl::function {
    template<typename R, typename ArgsPack>
    struct assemble_signature;

    template<typename R, typename... Args, template<typename...> typename Pack>
    struct assemble_signature<R, Pack<Args...>> {
        using type = R(Args...);
    };

    template<typename R, typename ArgsPack>
    using assemble_signature_t = assemble_signature<R, ArgsPack>::type;
}

#endif // VKZLIB_MPL_FUNCTION_ASSEMBLE_SIGNATURE_HPP