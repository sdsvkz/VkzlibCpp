#ifndef VKZLIB_INTERNAL_MIN_HPP
#define VKZLIB_INTERNAL_MIN_HPP

#include <cstddef>
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

#endif // VKZLIB_INTERNAL_MIN_HPP