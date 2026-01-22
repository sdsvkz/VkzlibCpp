#ifndef VKZLIB_MPL_COMMON_CE_UNROLL_HPP
#define VKZLIB_MPL_COMMON_CE_UNROLL_HPP

#include <vkzlib/mpl/common/Size.hpp>
#include <vkzlib/mpl/common/ce/foldl.hpp>

namespace vkz::mpl::ce {
    namespace _detail {
        template<auto F, typename R, Size... Is>
        consteval R _unroll_impl(R initial, SizeSequence<Is...>) {
            return foldl<[]<Size I>(R acc, SizeConstant<I>) -> R {
                return F(acc, SizeConstant<I>());
            }>(initial, SizeConstant<Is>()...);
        }
    }

    template<Size N, auto F, typename R>
    consteval R unroll(R initial) {
        return _detail::_unroll_impl<F>(initial, std::make_index_sequence<N>());
    }
}

#endif // VKZLIB_MPL_COMMON_CE_UNROLL_HPP