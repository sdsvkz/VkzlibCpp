#ifndef VKZLIB_MPL_FUNCTION_PARSE_HELPER_STL_FUNCTION_LIKE_HPP
#define VKZLIB_MPL_FUNCTION_PARSE_HELPER_STL_FUNCTION_LIKE_HPP

#include <vkzlib/mpl/function/parse/parse.hpp>
#include <vkzlib/mpl/function/parse/type/concepts.hpp>
#include <vkzlib/mpl/function/parse/type/enums.hpp>

namespace vkz::mpl::function::parse {
    template<type::IndirectInvocable T, typename S>
        requires (isAnyTypeOf<T>(type::DirectInvocableType::STLFunctionLike))
    using function_type_of_t = parse<T>::template _VKZLIB_F_T<S>;
}

#endif // VKZLIB_MPL_FUNCTION_PARSE_HELPER_STL_FUNCTION_LIKE_HPP