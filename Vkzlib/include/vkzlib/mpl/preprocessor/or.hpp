#ifndef VKZLIB_MPL_PREPROCESSOR_OR_HPP
#define VKZLIB_MPL_PREPROCESSOR_OR_HPP

#include <vkzlib/mpl/preprocessor/conditional.hpp>

#define VKZLIB_PP_OR(X, Y) VKZLIB_PP_CONDITIONAL(X, 1, Y)

#endif // VKZLIB_MPL_PREPROCESSOR_OR_HPP