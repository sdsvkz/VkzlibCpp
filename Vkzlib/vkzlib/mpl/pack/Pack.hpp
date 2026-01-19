#ifndef VKZLIB_INTERNAL_PACK_PACK_HPP
#define VKZLIB_INTERNAL_PACK_PACK_HPP

#include <cstddef>

#include <vkzlib/mpl/pack/nth_of.hpp>

namespace vkz::mpl::pack {
    /**
     * @brief Simple struct for holding template parameter pack.
     */
    template<typename... Ts>
    struct Pack {
        static constexpr std::size_t size = sizeof...(Ts);
    };

    template<typename First, typename... Rest>
    struct Pack<First, Rest...> {
        static constexpr std::size_t size = 1 + sizeof...(Rest);

        template<std::size_t N> requires (N < size)
        using At = nth_of_t<N, First, Rest...>;
    };
}

#endif // VKZLIB_INTERNAL_PACK_PACK_HPP