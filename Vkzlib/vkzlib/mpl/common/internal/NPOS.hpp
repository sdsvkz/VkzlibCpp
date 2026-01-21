#ifndef VKZLIB_INTERNAL_CONSTANT_HPP
#define VKZLIB_INTERNAL_CONSTANT_HPP

#include <numeric>

namespace vkz::mpl::internal {
    /**
     * @brief Constant representing invalid index. Alias of `std::numeric_limits<std::size_t>::max()`
     */
    inline constexpr std::size_t NPOS = std::numeric_limits<std::size_t>::max();
}

#endif // VKZLIB_INTERNAL_CONSTANT_HPP