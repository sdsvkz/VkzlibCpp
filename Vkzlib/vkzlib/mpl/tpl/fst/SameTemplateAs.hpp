#ifndef VKZLIB_MPL_TPL_FST_SAMETEMPLATEAS_HPP
#define VKZLIB_MPL_TPL_FST_SAMETEMPLATEAS_HPP

#include <type_traits>

namespace vkz::mpl::tpl::fst {
    template<template<typename...> typename, template<typename...> typename>
    struct is_same_template : std::false_type {};

    template<template<typename...> typename T1T>
    struct is_same_template<T1T, T1T> : std::true_type {};

    template<template<typename...> typename T1T, template<typename...> typename T1U>
    inline constexpr bool is_same_template_v = is_same_template<T1T, T1U>::value;

    namespace _impl {
        template<template<typename...> typename T1T, template<typename...> typename T1U>
        concept SameTemplateAs = is_same_template<T1T, T1U>::value;;
    }

    template<template<typename...> typename T1T, template<typename...> typename T1U>
    concept SameTemplateAs = _impl::SameTemplateAs<T1T, T1U> && _impl::SameTemplateAs<T1U, T1T>;
}

#endif // VKZLIB_MPL_TPL_FST_SAMETEMPLATEAS_HPP