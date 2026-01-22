#ifndef VKZLIB_COMMON_SIZE_HPP
#define VKZLIB_COMMON_SIZE_HPP

#include <cstddef>
#include <type_traits>
#include <utility>

namespace vkz::mpl {
    /**
     * @brief Type of size, default is `std::size_t`
     */
    using Size = std::size_t;

    template<Size N>
    using SizeConstant = std::integral_constant<Size, N>;

    template<Size... Ns>
    using SizeSequence = std::integer_sequence<Size, Ns...>;

    template<Size N>
    using MakeSizeSequence = std::make_integer_sequence<Size, N>;
}

#endif // VKZLIB_COMMON_SIZE_HPP