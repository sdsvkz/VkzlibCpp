#ifndef VKZLIB_MPL_COMMON_HOMOGENEOUS_HPP
#define VKZLIB_MPL_COMMON_HOMOGENEOUS_HPP

#include <concepts>

namespace vkz::mpl {
    /**
    * @brief All types are identical
    */
    template<typename T, typename... Us>
    concept Homogeneous = (std::same_as<T, Us> && ...);
}

#endif // VKZLIB_MPL_COMMON_HOMOGENEOUS_HPP