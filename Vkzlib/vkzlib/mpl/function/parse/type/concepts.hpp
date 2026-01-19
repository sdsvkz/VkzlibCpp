#ifndef VKZLIB_MPL_FUNCTION_TYPE_CONCEPTS_HPP
#define VKZLIB_MPL_FUNCTION_TYPE_CONCEPTS_HPP

#include <concepts>

#include <vkzlib/mpl/function/parse/helper.hpp>
#include <vkzlib/mpl/function/parse/type/enums.hpp>

namespace vkz::mpl::function::parse::type {
    // Categories

    template<typename T>
    concept NonInvocable = Parsable<T> &&
        std::same_as<category_of_t<T>, NonInvocableType>;

    template<typename T>
    concept DirectInvocable = Parsable<T> &&
        std::same_as<category_of_t<T>, DirectInvocableType>;

    template<typename T>
    concept IndirectInvocable = Parsable<T> &&
        std::same_as<category_of_t<T>, IndirectInvocableType>;

    // Concrete types

    /**
     * @brief Types like `R(Args...)`
     */
    template<typename T>
    concept Signature = Parsable<T> &&
        isAnyTypeOf<T>(DirectInvocableType::Signature);

    template<typename T>
    concept FunctionPointer = Parsable<T> &&
        isAnyTypeOf<T>(DirectInvocableType::FunctionPointer);

    /**
     * @brief Types similar to `std::function`: First template parameter is a signature
     */
    template<typename T>
    concept STLFunctionLike = Parsable<T> &&
        isAnyTypeOf<T>(DirectInvocableType::STLFunctionLike);

    /**
     * @brief Types with call operator, but not overloaded
     */
    template<typename T>
    concept MonomorphicFunctor = Parsable<T> &&
        isAnyTypeOf<T>(DirectInvocableType::MonomorphicFunctor);

    template<typename T>
    concept MemberFunctionPointer = Parsable<T> &&
        isAnyTypeOf<T>(IndirectInvocableType::MemberFunctionPointer);

}

#endif //VKZLIB_MPL_FUNCTION_TYPE_CONCEPTS_HPP