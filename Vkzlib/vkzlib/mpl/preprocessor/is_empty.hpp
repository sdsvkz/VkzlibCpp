#ifndef VKZLIB_MPL_PREPROCESSOR_IS_EMPTY_HPP
#define VKZLIB_MPL_PREPROCESSOR_IS_EMPTY_HPP

#include <vkzlib/mpl/preprocessor/cat.hpp>
#include <vkzlib/mpl/preprocessor/va_opt.hpp>

#define _VKZLIB_PP_IS_EMPTY_IMPL_HAVE_SOMETHING 0
#define _VKZLIB_PP_IS_EMPTY_IMPL 1
/**
 * @brief Empty -> `1`, otherwise `0`
 */
#define VKZLIB_PP_IS_EMPTY(...)	VKZLIB_PP_CAT3_RIGHT(		\
	_VKZLIB_PP_IS,											\
	_EMPTY_IMPL,											\
	VKZLIB_PP_VA_OPT(_HAVE_SOMETHING, __VA_ARGS__)			\
)

#endif // VKZLIB_MPL_PREPROCESSOR_IS_EMPTY_HPP