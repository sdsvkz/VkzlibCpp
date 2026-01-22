#ifndef VKZLIB_MPL_COMMON_PACK_PACK_HPP
#define VKZLIB_MPL_COMMON_PACK_PACK_HPP

#include <vkzlib/mpl/common/Size.hpp>
#include <vkzlib/mpl/common/pack/nth_of.hpp>

namespace vkz::mpl::pack {
    /**
     * @brief Simple struct for holding template parameter pack.
     */
    template<typename... Ts>
    struct Pack {
        static constexpr Size size = sizeof...(Ts);
    };

    /**
     * @brief Simple struct for holding template parameter pack.
     */
    template<typename First, typename... Rest>
    struct Pack<First, Rest...> {
        static constexpr Size size = 1 + sizeof...(Rest);

        template<Size N> requires (N < size)
        using At = nth_of_t<N, First, Rest...>;
    };
}

#endif // VKZLIB_MPL_COMMON_PACK_PACK_HPP