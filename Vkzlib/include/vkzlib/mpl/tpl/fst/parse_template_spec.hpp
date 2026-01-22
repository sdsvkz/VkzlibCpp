#ifndef VKZLIB_MPL_TPL_FST_PARSE_TEMPLATE_SPEC_HPP
#define VKZLIB_MPL_TPL_FST_PARSE_TEMPLATE_SPEC_HPP

#include <type_traits>

#include <vkzlib/mpl/common/Size.hpp>
#include <vkzlib/mpl/common/pack/nth_of.hpp>

namespace vkz::mpl::tpl::fst {
    namespace _detail {
        template<typename>
        struct _parse_template_spec_impl : std::false_type {};

        template<template<typename...> typename _TT, typename... Ts>
        struct _parse_template_spec_impl<_TT<Ts...>> : std::true_type {
            static_assert(sizeof...(Ts) == 0);
            static constexpr Size argc = 0;

            template<typename... Us>
            using Template = _TT<Us...>;
        };

        template<template<typename...> typename _TT, typename T, typename... Ts>
        struct _parse_template_spec_impl<_TT<T, Ts...>> : std::true_type {
            static constexpr Size argc = 1 + sizeof...(Ts);

            template<typename U, typename... Us>
            using Template = _TT<U, Us...>;

            template<Size N>
            using At = pack::nth_of_t<N, T, Ts...>;
        };

        template<typename T>
        using _parse_template_spec = _parse_template_spec_impl<std::remove_cvref_t<T>>;
    }

    /**
     * @brief A fully instantiated template
     */
    template<typename T>
    concept TemplateSpec =
        _detail::_parse_template_spec<T>::value;

    /**
     * @brief How many template parameters the template `TT` CAN (but not necessarily) take
     */
    template<TemplateSpec P>
    inline constexpr Size tparam_count_v =
        _detail::_parse_template_spec<P>::argc;

    /**
     * @brief The template (Without proper template signature)
     */
    template<TemplateSpec P, typename... Ts>
    using template_of_t = _detail::_parse_template_spec<P>::template Pack<Ts...>;

    /**
     * @brief N-th type applied to template, starts from 0
     *
     * @note Requires at least 1 template parameter applied to P
     */
    template<Size N, TemplateSpec P>
        requires (tparam_count_v<P> > 0)
    using nth_tparam_of_t = _detail::_parse_template_spec<P>::template At<N>;
}

#endif // VKZLIB_MPL_TPL_FST_PARSE_TEMPLATE_SPEC_HPP