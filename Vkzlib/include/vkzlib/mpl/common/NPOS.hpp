#ifndef VKZLIB_COMMON_NPOS_HPP
#define VKZLIB_COMMON_NPOS_HPP

#include <limits>

#include <vkzlib/mpl/common/Size.hpp>

namespace vkz::mpl {
    /**
     * @brief Constant representing invalid index. Alias of `std::numeric_limits<Size>::max()`
     */
    inline constexpr Size NPOS = std::numeric_limits<Size>::max();
}

#endif // VKZLIB_COMMON_NPOS_HPP