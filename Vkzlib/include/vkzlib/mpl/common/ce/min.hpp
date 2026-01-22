#ifndef VKZLIB_COMMON_CE_MIN_HPP
#define VKZLIB_COMMON_CE_MIN_HPP

#include <concepts>

namespace vkz::mpl::ce {
    /**
     * @brief Constant evaluated minimal value
     * @return Minimal value
     */
    template<typename T, std::totally_ordered_with<T>... Ts>
    consteval decltype(auto) min(T x, Ts... xs) {
        T res = x;
        ((res = res < xs ? res : xs), ...);
        return res;
    }
}

#endif // VKZLIB_COMMON_CE_MIN_HPP