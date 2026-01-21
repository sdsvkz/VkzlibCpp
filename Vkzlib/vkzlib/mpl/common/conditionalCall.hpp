#ifndef VKZLIB_MPL_COMMON_CONDITIONALCALL_HPP
#define VKZLIB_MPL_COMMON_CONDITIONALCALL_HPP

#include <concepts>

namespace vkz::mpl {
    /**
     * @brief Conditional call `doTrue` or `doFalse` based on `B`
     *
     * @tparam B Boolean value decides which function to call
     * @param doTrue The block called when `B` is true
     * @param doFalse The block called when `B` is false
     * @param args Arguments for the function
     */
    template<bool B, typename... Args, std::invocable<Args...> TBlock, std::invocable<Args...> FBlock>
    constexpr decltype(auto) conditionalCall(TBlock &&doTrue, FBlock &&doFalse, Args &&... args)
        noexcept(std::is_nothrow_invocable_v<TBlock> && std::is_nothrow_invocable_v<FBlock>)
    {
        if constexpr (B) {
            return std::forward<TBlock>(doTrue)(std::forward<Args>(args)...);
        } else {
            return std::forward<FBlock>(doFalse)(std::forward<Args>(args)...);
        }
    }
}

#endif // VKZLIB_MPL_COMMON_CONDITIONALCALL_HPP