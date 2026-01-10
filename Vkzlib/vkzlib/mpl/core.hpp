#ifndef VKZLIB_MPL_CORE_H
#define VKZLIB_MPL_CORE_H
#pragma once

#include <concepts>
#include <type_traits>
#include <tuple>

namespace vkz::mpl {
    /**
    * @brief A class (same as `std::is_class_v<T>`)
    */
    template<typename T>
    concept Class = std::is_class_v<T>;

    /**
    * @brief C-style String
    */
    template<typename CharT, typename NormalCharT = std::decay_t<std::remove_cvref_t<CharT> > >
    concept CString =
            std::same_as<NormalCharT, char *> &&
            std::same_as<NormalCharT, signed char *> &&
            std::same_as<NormalCharT, unsigned char *> &&
            std::same_as<NormalCharT, wchar_t *> &&
            std::same_as<NormalCharT, char8_t *> &&
            std::same_as<NormalCharT, char16_t *> &&
            std::same_as<NormalCharT, char32_t *>;

    /**
    * @brief All types are identical
    */
    template<typename... Ts>
    concept Homogeneous = (std::same_as<std::tuple_element_t<0, std::tuple<Ts...> >, Ts> && ...);

    /**
     * @brief All types have a common type
     *
     * Use `std::common_type_t` to get the type
     */
    template<typename... Ts>
    concept HasCommonType = requires
    {
        { std::declval<std::common_type_t<Ts...> >() };
    };

    template<bool B, std::invocable<> TBlock, std::invocable<> FBlock,
        typename TBlockR = std::invoke_result_t<TBlock>,
        typename FBlockR = std::invoke_result_t<FBlock> >
    constexpr decltype(auto) conditionalCall(const TBlock &doTrue, const FBlock &doFalse) {
        if constexpr (B) {
            return doTrue();
        } else {
            return doFalse();
        }
    }
}

#endif // VKZLIB_MPL_CORE_H
