#ifndef VKZLIB_MPL_FUNCTION_TYPE_ENUMS_HPP
#define VKZLIB_MPL_FUNCTION_TYPE_ENUMS_HPP

#include <vkzlib/mpl/common/AnyOf.hpp>

namespace vkz::mpl::function::parse::type {
    enum class NonInvocableType : signed char {
    };

    enum class DirectInvocableType : signed char {
        Signature,
        FunctionPointer,
        STLFunctionLike,
        MonomorphicFunctor,
    };

    enum class IndirectInvocableType : signed char {
        MemberFunctionPointer,
    };

    template<typename T>
    concept ParsableTypeEnum = AnyOf<T, NonInvocableType, DirectInvocableType, IndirectInvocableType>;
}

#endif // VKZLIB_MPL_FUNCTION_TYPE_ENUMS_HPP