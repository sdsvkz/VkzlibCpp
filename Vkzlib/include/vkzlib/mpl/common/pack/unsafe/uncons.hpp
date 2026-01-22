#ifndef VKZLIB_MPL_COMMON_PACK_UNSAFE_UNCONS_HPP
#define VKZLIB_MPL_COMMON_PACK_UNSAFE_UNCONS_HPP

#include <type_traits>

namespace vkz::mpl::pack::unsafe {
    template<auto Consume, typename... Ts>
    consteval decltype(auto) uncons() {
        static_assert(sizeof...(Ts) > 0, "uncons: Unexpected empty parameter pack, at least 1 type required");
        return Consume(std::type_identity<Ts>{}...);
    }
}

#endif // VKZLIB_MPL_COMMON_PACK_UNSAFE_UNCONS_HPP