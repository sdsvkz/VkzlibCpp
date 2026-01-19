#ifndef VKZLIB_MPL_FUNCTION_PARSE_HELPER_MEMBER_FUNCTION_POINTER_HPP
#define VKZLIB_MPL_FUNCTION_PARSE_HELPER_MEMBER_FUNCTION_POINTER_HPP

#include <vkzlib/mpl/function/parse/parse.hpp>
#include <vkzlib/mpl/function/parse/type/concepts.hpp>
#include <vkzlib/mpl/function/parse/type/enums.hpp>

namespace vkz::mpl::function::parse {
    template<type::IndirectInvocable T>
        requires (isAnyTypeOf<T>(type::IndirectInvocableType::MemberFunctionPointer))
    using class_of_t = parse<T>::_VKZLIB_C_T;
}

#endif // VKZLIB_MPL_FUNCTION_PARSE_HELPER_MEMBER_FUNCTION_POINTER_HPP