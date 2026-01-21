#ifndef VKZLIB_MPL_PREPROCESSOR_IS_ZERO_HPP
#define VKZLIB_MPL_PREPROCESSOR_IS_ZERO_HPP

#include <vkzlib/mpl/preprocessor/cat.hpp>
#include <vkzlib/mpl/preprocessor/const.hpp>
#include <vkzlib/mpl/preprocessor/pick_second_or.hpp>

#define _VKZLIB_PP_IS_ZERO_IMPL_TRUE_0 VKZLIB_PP_CONST(1)
/**
* @brief Evaluate to `1` if `X` is `0`, otherwise `0`
*/
#define VKZLIB_PP_IS_ZERO(X) \
    VKZLIB_PP_PICK_SECOND_OR(0, VKZLIB_PP_CAT(_VKZLIB_PP_IS_ZERO_IMPL_TRUE_, X))

#endif // VKZLIB_MPL_PREPROCESSOR_IS_ZERO_HPP