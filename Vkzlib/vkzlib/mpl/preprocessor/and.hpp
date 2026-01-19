#ifndef VKZLIB_MPL_PREPROCESSOR_AND_HPP
#define VKZLIB_MPL_PREPROCESSOR_AND_HPP

#include <vkzlib/mpl/preprocessor/conditional.hpp>

#define VKZLIB_PP_AND(X, Y) VKZLIB_PP_CONDITIONAL(X, Y, 0)

#endif // VKZLIB_MPL_PREPROCESSOR_AND_HPP