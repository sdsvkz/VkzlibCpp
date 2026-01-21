#ifndef VKZLIB_MPL_COMMON_HASCOMMONTYPE_HPP
#define VKZLIB_MPL_COMMON_HASCOMMONTYPE_HPP

#include <type_traits>

namespace vkz::mpl {
    /**
     * @brief All types have a common type
     *
     * Use `std::common_type_t` to get the type
     */
    template<typename... Ts>
    concept HasCommonType = requires
    {
        { std::declval<std::common_type_t<Ts...>>() };
    };
}

#endif // VKZLIB_MPL_COMMON_HASCOMMONTYPE_HPP