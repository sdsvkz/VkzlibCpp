#ifndef VKZLIB_MPL_COMMON_ANYOF_HPP
#define VKZLIB_MPL_COMMON_ANYOF_HPP

#include <concepts>

namespace vkz::mpl {
    /**
     * @brief `T` is one of `Us`
     */
    template<typename T, typename... Us>
    concept AnyOf = (std::same_as<T, Us> || ...);
}

#endif // VKZLIB_MPL_COMMON_ANYOF_HPP