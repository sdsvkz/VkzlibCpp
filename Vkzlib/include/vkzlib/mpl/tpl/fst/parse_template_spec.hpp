#ifndef VKZLIB_MPL_TPL_FST_PARSE_TEMPLATE_SPEC_HPP
#define VKZLIB_MPL_TPL_FST_PARSE_TEMPLATE_SPEC_HPP

#include <type_traits>

#include <vkzlib/mpl/common/pack/nth_of.hpp>

namespace vkz::mpl::tpl::fst {
    namespace _detail {
        template<typename>
        struct _parse_template_spec : std::false_type {};

        template<template<typename...> typename _TT, typename... Ts>
        struct _parse_template_spec<_TT<Ts...>> : std::true_type {
            static_assert(sizeof...(Ts) == 0);
            static constexpr std::size_t argc = 0;

            template<typename... Us>
            using Template = _TT<Us...>;
        };

        template<template<typename...> typename _TT, typename T, typename... Ts>
        struct _parse_template_spec<_TT<T, Ts...>> : std::true_type {
            static constexpr std::size_t argc = 1 + sizeof...(Ts);

            template<typename U, typename... Us>
            using Template = _TT<U, Us...>;

            template<std::size_t N>
            using At = pack::nth_of_t<N, T, Ts...>;
        };
    }

    /**
     * @brief A fully instantiated template
     */
    template<typename T>
    concept TemplateSpec = _detail::_parse_template_spec<T>::value;

    /**
     * @brief How many template parameters the template `TT` CAN (but not necessarily) take
     */
    template<TemplateSpec P>
    inline constexpr std::size_t tparam_count_v = _detail::_parse_template_spec<P>::argc;

    /**
     * @brief The template (Without proper template signature)
     */
    template<TemplateSpec P, typename... Ts>
    using template_of_t = _detail::_parse_template_spec<P>::template Pack<Ts...>;

    /**
     * @brief N-th type in pack
     */
    template<std::size_t N, TemplateSpec P>
        requires (tparam_count_v<P> != 0)
    using nth_tparam_of_t = _detail::_parse_template_spec<P>::template At<N>;
}

#endif // VKZLIB_MPL_TPL_FST_PARSE_TEMPLATE_SPEC_HPP