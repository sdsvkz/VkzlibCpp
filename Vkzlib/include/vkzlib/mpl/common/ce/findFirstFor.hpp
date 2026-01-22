#ifndef VKZLIB_MPL_COMMON_CE_FINDFIRSTFOR_HPP
#define VKZLIB_MPL_COMMON_CE_FINDFIRSTFOR_HPP

#include <type_traits>
#include <utility>

#include <vkzlib/mpl/common/Size.hpp>
#include <vkzlib/mpl/common/NPOS.hpp>
#include <vkzlib/mpl/common/ce/min.hpp>

namespace vkz::mpl::ce {
    /**
      * @brief Find the first `I` satisfy `Predicate` for `I` in `0 <= I < N`
      * @tparam Predicate NTTP `<Size I>(std::integral_constant<Size, I>) -> bool`
      * @return The number if found. `NPOS` otherwise
      */
    template<Size N, auto Predicate>
    consteval Size findFirstFor() {
        return []<Size... Is>(std::index_sequence<Is...>) {
            return min(NPOS, (Predicate(std::integral_constant<Size, Is>()) ? Is : NPOS)...);
        }(std::make_index_sequence<N>());
    }
}

#endif // VKZLIB_MPL_COMMON_CE_FINDFIRSTFOR_HPP