#ifndef VKZLIB_MPL_COMMON_CE_FINDFIRSTFOR_HPP
#define VKZLIB_MPL_COMMON_CE_FINDFIRSTFOR_HPP

#include <vkzlib/mpl/common/Size.hpp>
#include <vkzlib/mpl/common/NPOS.hpp>
#include <vkzlib/mpl/common/ce/min.hpp>

namespace vkz::mpl::ce {
    /**
      * @brief Find the first `I` satisfy `Predicate` for `I` in `0 <= I < N`
      * @tparam Predicate NTTP `<Size I>(SizeConstant<I>) -> bool`
      * @return The number if found. `NPOS` otherwise
      */
    template<Size N, auto Predicate>
    consteval Size findFirstFor() {
        return []<Size... Is>(SizeSequence<Is...>) {
            return min(NPOS, (Predicate(SizeConstant<Is>()) ? Is : NPOS)...);
        }(MakeSizeSequence<N>());
    }
}

#endif // VKZLIB_MPL_COMMON_CE_FINDFIRSTFOR_HPP