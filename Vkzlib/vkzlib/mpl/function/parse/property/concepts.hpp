#ifndef VKZLIB_MPL_FUNCTION_PARSE_PROPERTY_CONCEPTS_HPP
#define VKZLIB_MPL_FUNCTION_PARSE_PROPERTY_CONCEPTS_HPP

#include <concepts>

#include <vkzlib/mpl/function/parse/helper.hpp>
#include <vkzlib/mpl/function/property/enums.hpp>

namespace vkz::mpl::function::parse::property {
    template<std::equality_comparable T, std::convertible_to<T>... Ts>
    constexpr bool _any_equal(T x, Ts... ys) {
        return ((x == ys) || ...);
    }

    template<typename T>
    concept Variadic = Parsable<T> &&
        variadic_type_of<T> != function::property::VariadicParam::None;

    template<typename T>
    concept NonCV = Parsable<T> &&
        cv_qualifier_of<T> == function::property::CVQualifier::None;

    template<typename T>
    concept Const = Parsable<T> && _any_equal(cv_qualifier_of<T>,
        function::property::CVQualifier::Const,
        function::property::CVQualifier::ConstVolatile);

    template<typename T>
    concept Volatile = Parsable<T> && _any_equal(cv_qualifier_of<T>,
        function::property::CVQualifier::Volatile,
        function::property::CVQualifier::ConstVolatile);

    template<typename T>
    concept NonRef = Parsable<T> &&
        ref_qualifier_of<T> == function::property::RefQualifier::None;

    template<typename T>
    concept LValueRef = Parsable<T> &&
        ref_qualifier_of<T> == function::property::RefQualifier::LValue;

    template<typename T>
    concept RValueRef = Parsable<T> &&
        ref_qualifier_of<T> == function::property::RefQualifier::RValue;

    template<typename T>
    concept NoThrow = Parsable<T> &&
        exception_qualifier_of<T> == function::property::ExceptionQualifier::Noexcept;

    template<typename T>
    concept Normal = Parsable<T> &&
        NonCV<T> && NonRef<T> && !NoThrow<T>;
}

#endif // VKZLIB_MPL_FUNCTION_PARSE_PROPERTY_CONCEPTS_HPP