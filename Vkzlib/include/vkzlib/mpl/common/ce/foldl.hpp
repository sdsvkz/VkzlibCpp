#ifndef VKZLIB_MPL_COMMON_CE_FOLD_HPP
#define VKZLIB_MPL_COMMON_CE_FOLD_HPP

namespace vkz::mpl::ce {
    /**
     * Reduce variadic arguments with `F`
     *
     * This is `foldl'` from Haskell
     *
     * @tparam F `(Accumulator acc, T x) -> Accumulator`
     * @param acc initial value
     * @return reduced value
     */
    template<auto F, typename Accumulator>
    consteval Accumulator foldl(Accumulator acc) {
        return acc;
    }

    /**
     * Reduce variadic arguments with `F`
     *
     * This is `foldl'` from Haskell
     *
     * @tparam F `(Accumulator acc, T x) -> Accumulator`
     * @param acc initial value
     * @param x
     * @param xs Variadic arguments
     * @return reduced value
     */
    template<auto F, typename Accumulator, typename T, typename... Ts>
    consteval Accumulator foldl(Accumulator acc, T x, Ts... xs) {
        return foldl<F>(F(acc, x), xs...);
    }
}

#endif // VKZLIB_MPL_COMMON_CE_FOLD_HPP