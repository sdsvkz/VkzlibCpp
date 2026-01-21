#ifndef VKZLIB_MPL_COMMON_CE_UNROLL_HPP
#define VKZLIB_MPL_COMMON_CE_UNROLL_HPP

#include <cstddef>
#include <vkzlib/mpl/common/ce/foldl.hpp>

namespace vkz::mpl::ce {
    namespace _detail {
        template<auto F, typename R, std::size_t... Is>
        consteval R _unroll_impl(R initial, std::index_sequence<Is...>) {
            return foldl<[]<std::size_t I>(R acc, std::integral_constant<std::size_t, I>) -> R {
                return F(acc, std::integral_constant<std::size_t, I>());
            }>(initial, std::integral_constant<std::size_t, Is>()...);
        }
    }

    template<std::size_t N, auto F, typename R>
    consteval R unroll(R initial) {
        return _detail::_unroll_impl<F>(initial, std::make_index_sequence<N>());
    }
}

#endif // VKZLIB_MPL_COMMON_CE_UNROLL_HPP