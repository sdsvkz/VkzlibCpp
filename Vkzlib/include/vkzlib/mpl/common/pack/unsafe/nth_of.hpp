#ifndef VKZLIB_MPL_COMMON_PACK_UNSAFE_NTH_OF_HPP
#define VKZLIB_MPL_COMMON_PACK_UNSAFE_NTH_OF_HPP

#include <vkzlib/mpl/common/Size.hpp>

namespace vkz::mpl::pack::unsafe {
    template<Size N, typename... Ts>
    struct nth_of {
        static_assert(sizeof...(Ts) > 0, "nth_of (unsafe): Unexpected empty parameter pack, at least 1 type required");
    };

    template<Size N, typename T, typename... Ts>
    struct nth_of<N, T, Ts...> {
        using type = nth_of<N - 1, Ts...>::type;
    };

    template<typename T, typename... Ts>
    struct nth_of<0, T, Ts...> {
        using type = T;
    };

    template<Size N, typename... Ts>
    using nth_of_t = nth_of<N, Ts...>::type;
}

#endif // VKZLIB_MPL_COMMON_PACK_UNSAFE_NTH_OF_HPP