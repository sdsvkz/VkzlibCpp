#ifndef VKZLIB_MPL_PREPROCESSOR_VA_OPT_HPP
#define VKZLIB_MPL_PREPROCESSOR_VA_OPT_HPP

#include <vkzlib/mpl/preprocessor/cat.hpp>

/**
 * @brief Should behave the same as `__VA_OPT__`, but you need to forward `__VA_ARGS__` to it
 */
#define VKZLIB_PP_VA_OPT(X, ...) \
	__VA_OPT__(X)

#define _VKZLIB_VA_OPT_SUPPORTED_IMPL_1 1
#define _VKZLIB_VA_OPT_SUPPORTED_IMPL_0 0
#define _VKZLIB_VA_OPT_SUPPORTED_IMPL___VA_OPT__(X) 0
/**
 * @brief `1` if __VA_OPT__ is supported, otherwise `0`
 */
#define VKZLIB_VA_OPT_SUPPORTED VKZLIB_PP_CAT(_VKZLIB_VA_OPT_SUPPORTED_IMPL_, VKZLIB_PP_VA_OPT(1, 0))

static_assert(VKZLIB_VA_OPT_SUPPORTED,
	"\n__VA_OPT__ not supported. Be sure to set standard to C++20.\n"
	"Note that MSVC requires /Zc:preprocessor to make __VA_OPT__ available");

#endif // VKZLIB_MPL_PREPROCESSOR_VA_OPT_HPP