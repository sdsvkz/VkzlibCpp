#ifndef VKZLIB_MPL_PREPROCESSOR_SIGNATURE_UNTAG_HPP
#define VKZLIB_MPL_PREPROCESSOR_SIGNATURE_UNTAG_HPP

#include <vkzlib/mpl/preprocessor/cat.hpp>
#include <vkzlib/mpl/preprocessor/empty.hpp>

#define _VKZLIB_PP_SIGNATURE_UNTAG_IMPL_None VKZLIB_PP_EMPTY()

#define _VKZLIB_PP_SIGNATURE_UNTAG_IMPL_Variadic , ...

#define _VKZLIB_PP_SIGNATURE_UNTAG_IMPL_Const const
#define _VKZLIB_PP_SIGNATURE_UNTAG_IMPL_Volatile volatile
#define _VKZLIB_PP_SIGNATURE_UNTAG_IMPL_ConstVolatile const volatile

// NOLINTBEGIN(*-macro-parentheses)
#define _VKZLIB_PP_SIGNATURE_UNTAG_IMPL_LValue &
// NOLINTEND(*-macro-parentheses)
#define _VKZLIB_PP_SIGNATURE_UNTAG_IMPL_RValue &&

#define _VKZLIB_PP_SIGNATURE_UNTAG_IMPL_Noexcept noexcept

/* Evaluate tag to corresponding qualifier: `LValue` -> `&` */
#define VKZLIB_PP_SIGNATURE_UNTAG(TAG) VKZLIB_PP_CAT(_VKZLIB_PP_SIGNATURE_UNTAG_IMPL_, TAG)

#endif // VKZLIB_MPL_PREPROCESSOR_SIGNATURE_UNTAG_HPP