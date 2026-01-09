#ifndef VKZLIB_PREPROCESSOR_H
#define VKZLIB_PREPROCESSOR_H
#pragma once

#include <boost/preprocessor.hpp>

#define _VKZLIB_NORMALIZE_SPEC_IMPL_NOEX(TEMPLATE_SIGNATURE, STRUCT_NAME, NORMAL_TYPE, SUPER_CLASS, STRUCT_BODY, CV_OPT, REF_OPT, NOEX_OPT)	\
	VKZLIB_REM_CTOR TEMPLATE_SIGNATURE																										\
	struct STRUCT_NAME<																														\
		VKZLIB_REM_CTOR NORMAL_TYPE CV_OPT REF_OPT NOEX_OPT																					\
	> : SUPER_CLASS STRUCT_BODY;

// Applied exception specification: (none), noexcept
#define _VKZLIB_NORMALIZE_SPEC_IMPL_REF(TEMPLATE_SIGNATURE, STRUCT_NAME, NORMAL_TYPE, SUPER_CLASS, STRUCT_BODY, CV_OPT, REF_OPT)			\
	_VKZLIB_NORMALIZE_SPEC_IMPL_NOEX(TEMPLATE_SIGNATURE, STRUCT_NAME, NORMAL_TYPE, SUPER_CLASS, STRUCT_BODY, CV_OPT, REF_OPT,			)	\
	_VKZLIB_NORMALIZE_SPEC_IMPL_NOEX(TEMPLATE_SIGNATURE, STRUCT_NAME, NORMAL_TYPE, SUPER_CLASS, STRUCT_BODY, CV_OPT, REF_OPT, noexcept)

// Applied ref-qualification: (none), &, &&
#define _VKZLIB_NORMALIZE_SPEC_IMPL_CV(TEMPLATE_SIGNATURE, STRUCT_NAME, NORMAL_TYPE, SUPER_CLASS, STRUCT_BODY, CV_OPT)	\
	_VKZLIB_NORMALIZE_SPEC_IMPL_REF(TEMPLATE_SIGNATURE, STRUCT_NAME, NORMAL_TYPE, SUPER_CLASS, STRUCT_BODY, CV_OPT,   )	\
	_VKZLIB_NORMALIZE_SPEC_IMPL_REF(TEMPLATE_SIGNATURE, STRUCT_NAME, NORMAL_TYPE, SUPER_CLASS, STRUCT_BODY, CV_OPT, & )	\
	_VKZLIB_NORMALIZE_SPEC_IMPL_REF(TEMPLATE_SIGNATURE, STRUCT_NAME, NORMAL_TYPE, SUPER_CLASS, STRUCT_BODY, CV_OPT, &&)

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
* Usage:
* 
* `VKZLIB_NORMALIZE_SPEC((template<typename R, typename ...Args>), normalize_function_signature, (R(Args...)))`
* 
* @param TEMPLATE_SIGNATURE Template signature with parentheses.
*	e.g. `(template<typename R, typename ...Args>)`
* @param STRUCT_NAME Name of the struct to be specialized
* @param NORMAL_TYPE The type without cv/ref/noexcept qualifiers following, with parentheses.
*	e.g. `(R(Args...))`
* @param SUPER_CLASS class it inherits from. e.g. `std::true_type`
* @param STRUCT_BODY Body of the struct
*/
#define VKZLIB_PP_NORMALIZE_SPEC(TEMPLATE_SIGNATURE, STRUCT_NAME, NORMAL_TYPE, SUPER_CLASS, STRUCT_BODY)							\
	_VKZLIB_NORMALIZE_SPEC_IMPL_CV(TEMPLATE_SIGNATURE, STRUCT_NAME, NORMAL_TYPE, SUPER_CLASS, STRUCT_BODY,					)	\
	_VKZLIB_NORMALIZE_SPEC_IMPL_CV(TEMPLATE_SIGNATURE, STRUCT_NAME, NORMAL_TYPE, SUPER_CLASS, STRUCT_BODY, const			)	\
	_VKZLIB_NORMALIZE_SPEC_IMPL_CV(TEMPLATE_SIGNATURE, STRUCT_NAME, NORMAL_TYPE, SUPER_CLASS, STRUCT_BODY, volatile			)	\
	_VKZLIB_NORMALIZE_SPEC_IMPL_CV(TEMPLATE_SIGNATURE, STRUCT_NAME, NORMAL_TYPE, SUPER_CLASS, STRUCT_BODY, const volatile	)

/**
* Helper macro to generate trait specifications that remove cv-qualifiers, noexcept, references
*/
#define VKZLIB_NORMALIZE_TRAIT_SPEC(TEMPLATE_SIGNATURE, STRUCT_NAME, NORMAL_TYPE)	\
	VKZLIB_PP_NORMALIZE_SPEC(TEMPLATE_SIGNATURE, STRUCT_NAME, NORMAL_TYPE,				\
		std::true_type, {															\
			using type = VKZLIB_REM_CTOR NORMAL_TYPE;								\
		}																			\
	)

#endif // VKZLIB_PREPROCESSOR_H
