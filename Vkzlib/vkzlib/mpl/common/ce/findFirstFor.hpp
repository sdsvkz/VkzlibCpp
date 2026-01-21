#ifndef VKZLIB_INTERNAL_FINDINDEX_HPP
#define VKZLIB_INTERNAL_FINDINDEX_HPP

#include <type_traits>
#include <utility>

#include <vkzlib/mpl/common/internal/NPOS.hpp>
#include <vkzlib/mpl/common/ce/min.hpp>

namespace vkz::mpl::ce {
    /**
      * @brief Find the first `I` satisfy `Predicate` for `I` in `0 <= I < N`
      * @tparam Predicate NTTP `<size_t I>(std::integral_constant<std::size_t, I>) -> bool`
      * @return The number if found. `vkz::internal::NPOS` otherwise
      */
    template<std::size_t N, auto Predicate>
    consteval std::size_t findFirstFor() {
        return []<std::size_t... Is>(std::index_sequence<Is...>) {
            return min(internal::NPOS, (Predicate(std::integral_constant<std::size_t, Is>()) ? Is : internal::NPOS)...);
        }(std::make_index_sequence<N>());
    }
}

#endif // VKZLIB_INTERNAL_FINDINDEX_HPP