#ifndef VKZLIB_MPL_FUNCTION_PROPERTY_ENUMS_HPP
#define VKZLIB_MPL_FUNCTION_PROPERTY_ENUMS_HPP

namespace vkz::mpl::function::property {
    enum class VariadicParam : signed char {
        None,
        Variadic,
    };

    enum class CVQualifier : signed char {
        None,
        Const,
        Volatile,
        ConstVolatile,
    };

    enum class RefQualifier : signed char {
        None,
        LValue,
        RValue,
    };

    enum class ExceptionQualifier : signed char {
        None,
        Noexcept,
    };
}

#endif // VKZLIB_MPL_FUNCTION_PROPERTY_ENUMS_HPP