#ifndef VKZLIB_PREPROCESSOR_H
#define VKZLIB_PREPROCESSOR_H
#pragma once

#include <boost/preprocessor.hpp>

#define VKZLIB_PP_EMPTY()

/**
* @brief Helper to remove parenthesis, `(typename T, typename U)` -> `typename T, typename U`
*/
#define VKZLIB_PP_REMOVE_PARENTHESIS(...) __VA_ARGS__

#define _VKZLIB_PP_IS_EMPTY_IMPL_HAVE_SOMETHING 0
#define _VKZLIB_PP_IS_EMPTY_IMPL 1
/**
* @brief Empty -> `1`, otherwise `0`
*/
#define VKZLIB_PP_IS_EMPTY(...) _VKZLIB_PP_IS_EMPTY_IMPL##__VA_OPT__(_HAVE_SOMETHING)

#define _VKZLIB_PP_CAT_IMPL(X, Y) X##Y
/**
* @brief Concatenate two tokens (So that no whitespace is needed to separate them)
*/
#define VKZLIB_PP_CAT(X, Y) _VKZLIB_PP_CAT_IMPL(X, Y)

/**
* @brief Leave the argument unchanged
*/
#define VKZLIB_PP_IDENTITY(X) X

/** 
* @brief Expands to 2 arguments.First is some dummy placeholder that should be ignored, second is the value
*/
#define VKZLIB_PP_CONST(X) ~, X

// Pick second one
#define _VKZLIB_PP_PICK_SECOND_OR_IMPL(X, Y, ...) Y
/**
* @brief
* Pick second one, or X if there is no second one
* 
* Combine this with `VKZLIB_PP_CONST` to achieve simple token equality check
* 
* See implementation of `VKZLIB_PP_IS_ZERO`
*/
#define VKZLIB_PP_PICK_SECOND_OR(X, ...) _VKZLIB_PP_PICK_SECOND_OR_IMPL(__VA_ARGS__, X)

#define _VKZLIB_PP_IS_ZERO_IMPL_TRUE_0 VKZLIB_PP_CONST(1)
/**
* @brief Evalute to `1` if `X` is `0`, otherwise `0`
*/
#define VKZLIB_PP_IS_ZERO(X) \
    VKZLIB_PP_PICK_SECOND_OR(0, VKZLIB_PP_CAT(_VKZLIB_PP_IS_ZERO_IMPL_TRUE_, X))

#define _VKZLIB_PP_IF_IMPL_0(TRUE_MACRO, FALSE_MACRO) TRUE_MACRO
#define _VKZLIB_PP_IF_IMPL_1(TRUE_MACRO, FALSE_MACRO) FALSE_MACRO
#define VKZLIB_PP_IF(CONDITION, TRUE_MACRO, FALSE_MACRO) \
	VKZLIB_PP_CAT(_VKZLIB_PP_IF_IMPL_, VKZLIB_PP_IS_ZERO(CONDITION)) (TRUE_MACRO, FALSE_MACRO)

#define VKZLIB_PP_NOT(X) VKZLIB_PP_IF(X, 0, 1)

// ============== Normalize spec generation ==============

// ============== Signature Tags ==============

#define VKZLIB_PP_SIGNATURE_NONE_TAG None

#define VKZLIB_PP_SIGNATURE_CONST_TAG Const
#define VKZLIB_PP_SIGNATURE_VOLATILE_TAG Volatile
#define VKZLIB_PP_SIGNATURE_CONST_VOLATILE_TAG ConstVolatile

#define VKZLIB_PP_SIGNATURE_LVALUE_TAG LValue
#define VKZLIB_PP_SIGNATURE_RVALUE_TAG RValue

#define VKZLIB_PP_SIGNATURE_NOEXCEPT_TAG Noexcept

// ============== Equality check for Tags ==============

// Those macro evalute to `1` if `X` is desired tag, otherwise `0`

#define _VKZLIB_PP_SIGNATURE_IS_NONE_IMPL_TRUE_None VKZLIB_PP_CONST(1)
#define VKZLIB_PP_SIGNATURE_IS_NONE(X) \
	VKZLIB_PP_PICK_SECOND_OR(0, VKZLIB_PP_CAT(_VKZLIB_PP_SIGNATURE_IS_NONE_IMPL_TRUE_, X))

#define _VKZLIB_PP_SIGNATURE_IS_CONST_IMPL_TRUE_Const VKZLIB_PP_CONST(1)
#define VKZLIB_PP_SIGNATURE_IS_CONST(X) \
	VKZLIB_PP_PICK_SECOND_OR(0, VKZLIB_PP_CAT(_VKZLIB_PP_SIGNATURE_IS_CONST_IMPL_TRUE_, X))
#define _VKZLIB_PP_SIGNATURE_IS_VOLATILE_IMPL_TRUE_Volatile VKZLIB_PP_CONST(1)
#define VKZLIB_PP_SIGNATURE_IS_VOLATILE(X) \
	VKZLIB_PP_PICK_SECOND_OR(0, VKZLIB_PP_CAT(_VKZLIB_PP_SIGNATURE_IS_VOLATILE_IMPL_TRUE_, X))
#define _VKZLIB_PP_SIGNATURE_IS_CONST_VOLATILE_IMPL_TRUE_ConstVolatile VKZLIB_PP_CONST(1)
#define VKZLIB_PP_SIGNATURE_IS_CONST_VOLATILE(X) \
	VKZLIB_PP_PICK_SECOND_OR(0, VKZLIB_PP_CAT(_VKZLIB_PP_SIGNATURE_IS_CONST_VOLATILE_IMPL_TRUE_, X))

#define _VKZLIB_PP_SIGNATURE_IS_LVALUE_IMPL_TRUE_LValue VKZLIB_PP_CONST(1)
#define VKZLIB_PP_SIGNATURE_IS_LVALUE(X) \
	VKZLIB_PP_PICK_SECOND_OR(0, VKZLIB_PP_CAT(_VKZLIB_PP_SIGNATURE_IS_LVALUE_IMPL_TRUE_, X))
#define _VKZLIB_PP_SIGNATURE_IS_RVALUE_IMPL_TRUE_RValue VKZLIB_PP_CONST(1)
#define VKZLIB_PP_SIGNATURE_IS_RVALUE(X) \
	VKZLIB_PP_PICK_SECOND_OR(0, VKZLIB_PP_CAT(_VKZLIB_PP_SIGNATURE_IS_RVALUE_IMPL_TRUE_, X))

#define _VKZLIB_PP_SIGNATURE_IS_NOEXCEPT_IMPL_TRUE_Noexcept VKZLIB_PP_CONST(1)
#define VKZLIB_PP_SIGNATURE_IS_NOEXCEPT(X) \
	VKZLIB_PP_PICK_SECOND_OR(0, VKZLIB_PP_CAT(_VKZLIB_PP_SIGNATURE_IS_NOEXCEPT_IMPL_TRUE_, X))

// =============== Untag ===============

#define _VKZLIB_PP_SIGNATURE_UNTAG_IMPL_None VKZLIB_PP_EMPTY()

#define _VKZLIB_PP_SIGNATURE_UNTAG_IMPL_Const const
#define _VKZLIB_PP_SIGNATURE_UNTAG_IMPL_Volatile volatile
#define _VKZLIB_PP_SIGNATURE_UNTAG_IMPL_ConstVolatile const volatile

#define _VKZLIB_PP_SIGNATURE_UNTAG_IMPL_LValue &
#define _VKZLIB_PP_SIGNATURE_UNTAG_IMPL_RValue &&

#define _VKZLIB_PP_SIGNATURE_UNTAG_IMPL_Noexcept noexcept

/* Evalute tag to corresponding qualifier: `LValue` -> `&` */
#define VKZLIB_PP_SIGNATURE_UNTAG(TAG) VKZLIB_PP_CAT(_VKZLIB_PP_SIGNATURE_UNTAG_IMPL_, TAG)

namespace vkz::preprocessor {
	enum class CVQualifier {
		VKZLIB_PP_SIGNATURE_NONE_TAG,
		VKZLIB_PP_SIGNATURE_CONST_TAG,
		VKZLIB_PP_SIGNATURE_VOLATILE_TAG,
		VKZLIB_PP_SIGNATURE_CONST_VOLATILE_TAG,
	};

	enum class RefQualifier {
		VKZLIB_PP_SIGNATURE_NONE_TAG,
		VKZLIB_PP_SIGNATURE_LVALUE_TAG,
		VKZLIB_PP_SIGNATURE_RVALUE_TAG,
	};

	enum class ExceptionQualifier {
		VKZLIB_PP_SIGNATURE_NONE_TAG,
		VKZLIB_PP_SIGNATURE_NOEXCEPT_TAG,
	};
}

#define _VKZLIB_NORMALIZE_SPEC_IMPL_NOEX(TEMPLATE_SIGNATURE, STRUCT_NAME, NORMAL_TYPE, SUPER_CLASS, STRUCT_BODY, CV_TAG, REF_TAG, NOEX_TAG)	  \
	VKZLIB_PP_REMOVE_PARENTHESIS TEMPLATE_SIGNATURE																							  \
	struct STRUCT_NAME<																														  \
		VKZLIB_PP_REMOVE_PARENTHESIS NORMAL_TYPE VKZLIB_PP_SIGNATURE_UNTAG(CV_TAG) VKZLIB_PP_SIGNATURE_UNTAG(REF_TAG) VKZLIB_PP_SIGNATURE_UNTAG(NOEX_TAG)																														    \
	> : SUPER_CLASS STRUCT_BODY(STRUCT_NAME, SUPER_CLASS, NORMAL_TYPE, CV_TAG, REF_TAG, NOEX_TAG);

// Applied exception specification: (none), noexcept
#define _VKZLIB_NORMALIZE_SPEC_IMPL_REF(TEMPLATE_SIGNATURE, STRUCT_NAME, NORMAL_TYPE, SUPER_CLASS, STRUCT_BODY, CV_TAG, REF_TAG)	\
	_VKZLIB_NORMALIZE_SPEC_IMPL_NOEX(TEMPLATE_SIGNATURE, STRUCT_NAME, NORMAL_TYPE, SUPER_CLASS, STRUCT_BODY, CV_TAG, REF_TAG,		\
		VKZLIB_PP_SIGNATURE_NONE_TAG																								\
	)																																\
	_VKZLIB_NORMALIZE_SPEC_IMPL_NOEX(TEMPLATE_SIGNATURE, STRUCT_NAME, NORMAL_TYPE, SUPER_CLASS, STRUCT_BODY, CV_TAG, REF_TAG,		\
		VKZLIB_PP_SIGNATURE_NOEXCEPT_TAG																							\
	)

// Applied ref-qualification: (none), &, &&
#define _VKZLIB_NORMALIZE_SPEC_IMPL_CV(TEMPLATE_SIGNATURE, STRUCT_NAME, NORMAL_TYPE, SUPER_CLASS, STRUCT_BODY, CV_TAG)	\
	_VKZLIB_NORMALIZE_SPEC_IMPL_REF(TEMPLATE_SIGNATURE, STRUCT_NAME, NORMAL_TYPE, SUPER_CLASS, STRUCT_BODY, CV_TAG,		\
		VKZLIB_PP_SIGNATURE_NONE_TAG																					\
	)																													\
	_VKZLIB_NORMALIZE_SPEC_IMPL_REF(TEMPLATE_SIGNATURE, STRUCT_NAME, NORMAL_TYPE, SUPER_CLASS, STRUCT_BODY, CV_TAG,		\
		VKZLIB_PP_SIGNATURE_LVALUE_TAG																					\
	)																													\
	_VKZLIB_NORMALIZE_SPEC_IMPL_REF(TEMPLATE_SIGNATURE, STRUCT_NAME, NORMAL_TYPE, SUPER_CLASS, STRUCT_BODY, CV_TAG,		\
		VKZLIB_PP_SIGNATURE_RVALUE_TAG																					\
	)

// Applied cv-qualification: (none), const, volatile, const volatile
/**
* @brief
* Helper macros to generate all combinations of cv/ref/noexcept specifications
* for types with function signature in it:
*
* - exception specification: (none), noexcept
*
* - ref-qualification: (none), &, &&
*
* - cv-qualification: (none), const, volatile, const volatile
*
* This will generate  2 * 4 * 3 = 24 specializations
*
* Usage: See `VKZLIB_NORMALIZE_TRAIT_SPEC`
*
* @param TEMPLATE_SIGNATURE Template signature with parentheses.
*	e.g. `(template<typename R, typename ...Args>)`
* @param STRUCT_NAME Name of the struct to be specialized
* @param NORMAL_TYPE The type without cv/ref/noexcept qualifiers following, with parentheses.
*	e.g. `(R(Args...))`
* @param SUPER_CLASS class it inherits from. e.g. `std::true_type`
* @param STRUCT_BODY Body of the struct. A higher-order macro that takes 4 parameters: NORMAL_TYPE, CV_TAG, REF_TAG, NOEX_TAG
*/
#define VKZLIB_PP_NORMALIZE_SPEC(TEMPLATE_SIGNATURE, STRUCT_NAME, NORMAL_TYPE, SUPER_CLASS, STRUCT_BODY)	\
	_VKZLIB_NORMALIZE_SPEC_IMPL_CV(TEMPLATE_SIGNATURE, STRUCT_NAME, NORMAL_TYPE, SUPER_CLASS, STRUCT_BODY,	\
		VKZLIB_PP_SIGNATURE_NONE_TAG																		\
	)																										\
	_VKZLIB_NORMALIZE_SPEC_IMPL_CV(TEMPLATE_SIGNATURE, STRUCT_NAME, NORMAL_TYPE, SUPER_CLASS, STRUCT_BODY,	\
		VKZLIB_PP_SIGNATURE_CONST_TAG																		\
	)																										\
	_VKZLIB_NORMALIZE_SPEC_IMPL_CV(TEMPLATE_SIGNATURE, STRUCT_NAME, NORMAL_TYPE, SUPER_CLASS, STRUCT_BODY,	\
		VKZLIB_PP_SIGNATURE_VOLATILE_TAG																	\
	)																										\
	_VKZLIB_NORMALIZE_SPEC_IMPL_CV(TEMPLATE_SIGNATURE, STRUCT_NAME, NORMAL_TYPE, SUPER_CLASS, STRUCT_BODY,	\
		VKZLIB_PP_SIGNATURE_CONST_VOLATILE_TAG																\
	)

#endif // VKZLIB_PREPROCESSOR_H
