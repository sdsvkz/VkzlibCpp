#ifndef VKZLIB_MPL_PREPROCESSOR_NOT_HPP
#define VKZLIB_MPL_PREPROCESSOR_NOT_HPP

#include <vkzlib/mpl/preprocessor/conditional.hpp>

#define VKZLIB_PP_NOT(X) VKZLIB_PP_CONDITIONAL(X, 0, 1)

#endif // VKZLIB_MPL_PREPROCESSOR_NOT_HPP