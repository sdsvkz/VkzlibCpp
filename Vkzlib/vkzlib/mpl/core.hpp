#ifndef VKZLIB_MPL_CORE_H
#define VKZLIB_MPL_CORE_H
#pragma once

#include <concepts>
#include <type_traits>

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
     * @brief `T` is one of `Us`
     */
    template<typename T, typename... Us>
    concept AnyOf = (std::same_as<T, Us> || ...);

    /**
    * @brief All types are identical
    */
    template<typename T, typename... Us>
    concept Homogeneous = (std::same_as<T, Us> && ...);

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

    template<bool B, std::invocable<> TBlock, std::invocable<> FBlock,
        typename TBlockR = std::invoke_result_t<TBlock>,
        typename FBlockR = std::invoke_result_t<FBlock>>
    constexpr decltype(auto) conditionalCall(TBlock &&doTrue, FBlock &&doFalse)
        noexcept(std::is_nothrow_invocable_v<TBlock> && std::is_nothrow_invocable_v<FBlock>)
    {
        if constexpr (B) {
            return doTrue();
        } else {
            return doFalse();
        }
    }

    template<typename From, typename To>
    struct dup_reference {
    private:
        using _RefRemoved = std::remove_reference_t<To>;
    public:
        using type =
            std::conditional_t<std::is_lvalue_reference_v<From>,
                std::add_lvalue_reference_t<_RefRemoved>,
                std::conditional_t<std::is_rvalue_reference_v<From>,
                    std::add_rvalue_reference_t<_RefRemoved>,
                    _RefRemoved>>;
    };

    template<typename From, typename To>
    using dup_reference_t = dup_reference<From, To>::type;

    template<typename From, typename To>
    struct dup_const {
    private:
        using _ConstRemoved = std::remove_const_t<To>;
    public:
        using type = std::conditional_t<std::is_const_v<From>,
            std::add_const_t<_ConstRemoved>,
            _ConstRemoved>;
    };

    template<typename From, typename To>
    using dup_const_t = dup_const<From, To>::type;

    template<typename From, typename To>
    struct dup_volatile {
    private:
        using _VolatileRemoved = std::remove_volatile_t<To>;
    public:
        using type = std::conditional_t<std::is_volatile_v<From>,
            std::add_volatile_t<_VolatileRemoved>,
            _VolatileRemoved>;
    };

    template<typename From, typename To>
    using dup_volatile_t = dup_volatile<From, To>::type;

    template<typename From, typename To>
    struct dup_cvref {
    private:
        using _Base = std::remove_cvref_t<To>;
        using _ConstHandled = std::conditional_t<
            std::is_const_v<std::remove_reference_t<From>>,
            std::add_const_t<_Base>,
            _Base>;
        using _CVHandled = std::conditional_t<
            std::is_volatile_v<std::remove_reference_t<From>>,
            std::add_volatile_t<_ConstHandled>,
            _ConstHandled>;
    public:
        using type = dup_reference_t<From, _CVHandled>;
    };

    template<typename From, typename To>
    using dup_cvref_t = dup_cvref<From, To>::type;

}

#endif // VKZLIB_MPL_CORE_H
