#ifndef VKZLIB_MPL_COMMON_CONDITIONALCALL_HPP
#define VKZLIB_MPL_COMMON_CONDITIONALCALL_HPP

#include <concepts>

namespace vkz::mpl {
    template<bool B, std::invocable<> TBlock, std::invocable<> FBlock>
    constexpr decltype(auto) conditionalCall(TBlock &&doTrue, FBlock &&doFalse)
        noexcept(std::is_nothrow_invocable_v<TBlock> && std::is_nothrow_invocable_v<FBlock>)
    {
        if constexpr (B) {
            return doTrue();
        } else {
            return doFalse();
        }
    }
}

#endif // VKZLIB_MPL_COMMON_CONDITIONALCALL_HPP