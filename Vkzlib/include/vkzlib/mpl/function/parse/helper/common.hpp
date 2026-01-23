#ifndef VKZLIB_MPL_FUNCTION_PARSE_HELPER_COMMON_HPP
#define VKZLIB_MPL_FUNCTION_PARSE_HELPER_COMMON_HPP

#include <type_traits>
#include <concepts>

#include <vkzlib/mpl/common/DefaultPack.hpp>
#include <vkzlib/mpl/function/parse/parse.hpp>

namespace vkz::mpl::function::parse {
    template<typename T>
    concept Parsable = parse<T>::value;

    template<Parsable T>
    inline constexpr auto type_of = parse<T>::_VKZLIB_P_T;

    template<Parsable T>
    using category_of_t = std::remove_cvref_t<decltype(type_of<T>)>;

    template<typename F, typename G>
    concept SameCategoryAs = std::same_as<category_of_t<F>, category_of_t<G>>;

    template<type::ParsableTypeEnum C, type::ParsableTypeEnum E>
    constexpr bool _isAnyTypeOf_impl(C type, E typeEnum) {
        if constexpr (std::same_as<C, E>) {
            return type == typeEnum;
        } else {
            return false;
        }
    }

    template<Parsable T, type::ParsableTypeEnum... Es>
    constexpr bool isAnyTypeOf(Es... typeEnums) {
        constexpr auto type = type_of<T>;
        return ((_isAnyTypeOf_impl(type, typeEnums)) || ...);
    }

    template<Parsable T>
    using result_of_t = parse<T>::_VKZLIB_R_T;

    template<Parsable T, template <typename...> typename Pack = DefaultPack>
    using args_of_t = parse<T>::template _VKZLIB_ARGS_PACK_T<Pack>;

    template<Parsable T>
    inline constexpr auto variadic_type_of = parse<T>::_VKZLIB_VAR;

    template<Parsable T>
    inline constexpr auto cv_qualifier_of = parse<T>::_VKZLIB_CV;

    template<Parsable T>
    inline constexpr auto ref_qualifier_of = parse<T>::_VKZLIB_REF;

    template<Parsable T>
    inline constexpr auto exception_qualifier_of = parse<T>::_VKZLIB_NOEX;

    /**
     * @note Lambda is not normalizable
     */
    template<typename T>
    concept Normalizable = Parsable<T> &&
        not std::same_as<std::remove_cvref_t<typename parse<T>::_VKZLIB_N_T>, void>;

    /**
     * @brief Remove CV, Ref, noexcept from signature
     *
     * @note Lambda is not normalizable
     */
    template<Normalizable T>
    using normalize_t = parse<T>::_VKZLIB_N_T;
}

#endif // VKZLIB_MPL_FUNCTION_PARSE_HELPER_COMMON_HPP