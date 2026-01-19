#ifndef VKZLIB_MPL_FUNCTION_SAMEARGSAS_HPP
#define VKZLIB_MPL_FUNCTION_SAMEARGSAS_HPP

#include <type_traits>
#include <concepts>

#include <vkzlib/mpl/function/internal/DefaultPack.hpp>
#include <vkzlib/mpl/function/parse/helper/common.hpp>
#include <vkzlib/mpl/function/parse/property/concepts.hpp>

namespace vkz::mpl::function {
    template<typename, typename>
        struct is_same_args_pack : std::false_type {};

    template<
        template<typename...> typename FPack,
        template<typename...> typename GPack,
        typename... FArgs, typename... GArgs>
    struct is_same_args_pack<FPack<FArgs...>, GPack<GArgs...>>
        : std::bool_constant<(std::same_as<FArgs, GArgs> && ...)> {};

    template<typename FArgsPack, typename GArgsPack>
    inline constexpr bool is_same_args_pack_v = is_same_args_pack<FArgsPack, GArgsPack>::value;

    /**
     * @brief Parsable function type with the same parameter list as `G`, including C variadic parameter (`...`).
     *
     * @tparam FPack Container for storing types of arguments, for `F`
     * @tparam GPack Same as `FPack`, but for `G`
     */
    template<typename F, typename G,
        template <typename...> typename FPack = DefaultPack,
        template <typename...> typename GPack = DefaultPack>
    concept SameArgsAs =
        parse::Parsable<F> &&
        parse::Parsable<G> &&
        parse::property::Variadic<F> == parse::property::Variadic<G> &&
        is_same_args_pack_v<parse::args_of_t<F, FPack>, parse::args_of_t<G, GPack>>;
}

#endif // VKZLIB_MPL_FUNCTION_SAMEARGSAS_HPP