#ifndef VKZLIB_MPL_COMMON_PACK_IS_EMPTY_PACK_HPP
#define VKZLIB_MPL_COMMON_PACK_IS_EMPTY_PACK_HPP

namespace vkz::mpl::pack {
    template<typename... Ts>
    inline constexpr bool is_empty_pack_v = sizeof...(Ts) == 0;
}

#endif // VKZLIB_MPL_COMMON_PACK_IS_EMPTY_PACK_HPP