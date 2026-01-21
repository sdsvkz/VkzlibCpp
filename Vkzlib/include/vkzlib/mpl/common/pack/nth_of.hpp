#ifndef VKZLIB_MPL_COMMON_PACK_NTH_OF_HPP
#define VKZLIB_MPL_COMMON_PACK_NTH_OF_HPP

#include <cstddef>

namespace vkz::mpl::pack {
    template<std::size_t N, typename First, typename... Rest>
        requires (N < 1 + sizeof...(Rest))
    struct nth_of {
        using type = nth_of<N - 1, Rest...>::type;
    };

    template<typename First, typename... Rest>
    struct nth_of<0, First, Rest...> {
        using type = First;
    };

    template<std::size_t N, typename First, typename... Rest>
    using nth_of_t = nth_of<N, First, Rest...>::type;
}

#endif // VKZLIB_MPL_COMMON_PACK_NTH_OF_HPP