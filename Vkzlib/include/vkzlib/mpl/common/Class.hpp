#ifndef VKZLIB_MPL_COMMON_CLASS_HPP
#define VKZLIB_MPL_COMMON_CLASS_HPP

#include <type_traits>

namespace vkz::mpl {
    /**
    * @brief A class (same as `std::is_class_v<T>`)
    */
    template<typename T>
    concept Class = std::is_class_v<T>;
}

#endif // VKZLIB_MPL_COMMON_CLASS_HPP