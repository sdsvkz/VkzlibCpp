#ifndef VKZLIB_MPL_FUNCTION_PARSE_PARSE_HPP
#define VKZLIB_MPL_FUNCTION_PARSE_PARSE_HPP

#include <type_traits>
#include <concepts>

#include <vkzlib/mpl/common/dup_cvref.hpp>
#include <vkzlib/mpl/function/MonomorphicFunctor.hpp>
#include <vkzlib/mpl/function/parse/type/enums.hpp>
#include <vkzlib/mpl/function/property/enums.hpp>
#include <vkzlib/mpl/preprocessor/and.hpp>
#include <vkzlib/mpl/preprocessor/expand_if.hpp>
#include <vkzlib/mpl/preprocessor/signature/is_tag.hpp>
#include <vkzlib/mpl/preprocessor/signature/map_syntax_product.hpp>
#include <vkzlib/mpl/preprocessor/signature/tag.hpp>
#include <vkzlib/mpl/preprocessor/signature/tag_to_enum.hpp>
#include <vkzlib/mpl/preprocessor/signature/untag.hpp>

namespace vkz::mpl::function::parse {

    template<typename T>
	struct _parse_impl : std::false_type {};

	// ============ Preprocessors ============

	// Parsable type enum field name
#define _VKZLIB_P_T type
	// Class type alias name
#define _VKZLIB_C_T Class
	// STL function like type alias name
#define _VKZLIB_F_T Function
	// Normal type alias name
#define _VKZLIB_N_T Normal
	// Return type alias name
#define _VKZLIB_R_T Result
	// Packed arguments type alias name
#define _VKZLIB_ARGS_PACK_T PackedArgs
	// VariadicParam enum field name
#define _VKZLIB_VAR variadic
	// CVQualifier enum field name
#define _VKZLIB_CV cv
	// RefQualifier enum field name
#define _VKZLIB_REF ref
	// ExceptionQualifier enum field name
#define _VKZLIB_NOEX noex
	// Template monomorphic functor type name
#define _VKZLIB_T_L L
	// Function template type name
#define _VKZLIB_T_F F
	// Template class type name
#define _VKZLIB_T_C C
	// Template return type name
#define _VKZLIB_T_R R
	// Template arguments parameter pack name
#define _VKZLIB_T_ARGS Args
	// Template parameter pack container type name
#define _VKZLIB_T_PACK Pack

#define VKZLIB_PARSER_TRAIT_SPEC_BODY_COMMON(										\
	VAR_TAG, CV_TAG, REF_TAG, NOEX_TAG												\
)																					\
	using _VKZLIB_R_T = _VKZLIB_T_R;												\
	template<template <typename...> typename Pack>									\
	using _VKZLIB_ARGS_PACK_T = Pack<_VKZLIB_T_ARGS...>;							\
	static constexpr auto _VKZLIB_VAR =												\
		property::VariadicParam::VKZLIB_PP_SIGNATURE_TAG_TO_ENUM(VAR_TAG);			\
	static constexpr auto _VKZLIB_CV =												\
		property::CVQualifier::VKZLIB_PP_SIGNATURE_TAG_TO_ENUM(CV_TAG);				\
	static constexpr auto _VKZLIB_REF =												\
		property::RefQualifier::VKZLIB_PP_SIGNATURE_TAG_TO_ENUM(REF_TAG);			\
	static constexpr auto _VKZLIB_NOEX =											\
		property::ExceptionQualifier::VKZLIB_PP_SIGNATURE_TAG_TO_ENUM(NOEX_TAG);

	/* Generate specializations:
	 *	 1. Create a macro that will expand into the target type.
	 *		Use `VKZLIB_PP_SIGNATURE_UNTAG` to expand a tag into qualifiers.
	 *		Note that `VAR_TAG` expands to `, ...`
	 */
#define VKZLIB_FUNCTION_SIGNATURE_TYPE(									\
	VAR_TAG, CV_TAG, REF_TAG, NOEX_TAG									\
)																		\
	_VKZLIB_T_R(_VKZLIB_T_ARGS... VKZLIB_PP_SIGNATURE_UNTAG(VAR_TAG))	\
		VKZLIB_PP_SIGNATURE_UNTAG(CV_TAG)								\
		VKZLIB_PP_SIGNATURE_UNTAG(REF_TAG)								\
		VKZLIB_PP_SIGNATURE_UNTAG(NOEX_TAG)

	// ============ Function Signature ============

	/* Generate specializations:
	 *	 2. Create a macro that will expand into a specializations.
	 *		You can combine `VKZLIB_PP_CONDITIONAL` and `VKZLIB_PP_SIGNATURE_IS_*` macros
	 *		to branch contents based on tags
	 */
#define VKZLIB_DEFINE_PARSE_FUNCTION_SIGNATURE(							\
	VAR_TAG, CV_TAG, REF_TAG, NOEX_TAG									\
)																		\
	template<															\
		typename _VKZLIB_T_R, typename ..._VKZLIB_T_ARGS				\
	> struct _parse_impl<VKZLIB_FUNCTION_SIGNATURE_TYPE(				\
		VAR_TAG, CV_TAG, REF_TAG, NOEX_TAG								\
	)> : std::true_type													\
	{																	\
		static constexpr auto _VKZLIB_P_T =								\
			function::parse::type::DirectInvocableType::Signature;		\
		using _VKZLIB_N_T = VKZLIB_FUNCTION_SIGNATURE_TYPE(				\
			VAR_TAG,													\
			VKZLIB_PP_SIGNATURE_NONE_TAG,								\
			VKZLIB_PP_SIGNATURE_NONE_TAG,								\
			VKZLIB_PP_SIGNATURE_NONE_TAG								\
		);																\
		VKZLIB_PARSER_TRAIT_SPEC_BODY_COMMON(							\
			VAR_TAG, CV_TAG, REF_TAG, NOEX_TAG							\
		)																\
	};

	/* Generate specializations:
	 *	 3. This will apply all combinations of tags to your higher-order macro
	 */
	VKZLIB_PP_SIGNATURE_MAP_SYNTAX_PRODUCT(VKZLIB_DEFINE_PARSE_FUNCTION_SIGNATURE)

#undef VKZLIB_DEFINE_PARSE_FUNCTION_SIGNATURE

	// ============ Function Pointer ============

#define VKZLIB_FUNCTION_POINTER_TYPE(											\
	VAR_TAG, CV_TAG, REF_TAG, NOEX_TAG											\
)																				\
	_VKZLIB_T_R(*)(_VKZLIB_T_ARGS... VKZLIB_PP_SIGNATURE_UNTAG(VAR_TAG))		\
		VKZLIB_PP_SIGNATURE_UNTAG(NOEX_TAG)

#define _VKZLIB_DEFINE_PARSE_FUNCTION_POINTER_IMPL(							\
	VAR_TAG, CV_TAG, REF_TAG, NOEX_TAG										\
)																			\
(																			\
	template<																\
		typename _VKZLIB_T_R, typename ..._VKZLIB_T_ARGS					\
	> struct _parse_impl<VKZLIB_FUNCTION_POINTER_TYPE(						\
		VAR_TAG, CV_TAG, REF_TAG, NOEX_TAG									\
	)> : _parse_impl<VKZLIB_FUNCTION_SIGNATURE_TYPE(						\
		VAR_TAG, CV_TAG, REF_TAG, NOEX_TAG									\
	)>																		\
	{																		\
		static constexpr auto _VKZLIB_P_T =									\
			function::parse::type::DirectInvocableType::FunctionPointer;	\
		using _VKZLIB_N_T = VKZLIB_FUNCTION_POINTER_TYPE(					\
			VAR_TAG,														\
			VKZLIB_PP_SIGNATURE_NONE_TAG,									\
			VKZLIB_PP_SIGNATURE_NONE_TAG,									\
			VKZLIB_PP_SIGNATURE_NONE_TAG									\
		);																	\
	};																		\
)

	/* Generate specializations:
	 *	 This is what you can do if you don't want to generate some specializations.
	 */
#define VKZLIB_DEFINE_PARSE_FUNCTION_POINTER(				\
	VAR_TAG, CV_TAG, REF_TAG, NOEX_TAG						\
)															\
	VKZLIB_PP_EXPAND_TUPLE_IF(								\
		VKZLIB_PP_AND(										\
			VKZLIB_PP_SIGNATURE_IS_NONE(CV_TAG),			\
			VKZLIB_PP_SIGNATURE_IS_NONE(REF_TAG)			\
		),													\
		_VKZLIB_DEFINE_PARSE_FUNCTION_POINTER_IMPL(			\
			VAR_TAG, CV_TAG, REF_TAG, NOEX_TAG				\
		)													\
	)

	// Total: 2 * 2 = 4
	VKZLIB_PP_SIGNATURE_MAP_SYNTAX_PRODUCT(VKZLIB_DEFINE_PARSE_FUNCTION_POINTER)

#undef VKZLIB_DEFINE_PARSE_FUNCTION_POINTER
#undef _VKZLIB_DEFINE_PARSE_FUNCTION_POINTER_IMPL
#undef VKZLIB_FUNCTION_POINTER_TYPE

	// ============ std::function Like ============

#define VKZLIB_STD_FUNCTION_LIKE_TYPE(							\
	VAR_TAG, CV_TAG, REF_TAG, NOEX_TAG, OTHERS					\
)																\
	_VKZLIB_T_F<VKZLIB_FUNCTION_SIGNATURE_TYPE(					\
		VAR_TAG, CV_TAG, REF_TAG, NOEX_TAG						\
	), OTHERS...>

#define VKZLIB_DEFINE_PARSE_STD_FUNCTION_LIKE(								\
	VAR_TAG, CV_TAG, REF_TAG, NOEX_TAG										\
)																			\
	template<																\
		template <typename, typename...> typename _VKZLIB_T_F,				\
		typename... Others,													\
		typename _VKZLIB_T_R, typename... _VKZLIB_T_ARGS					\
	> requires																\
		std::invocable<VKZLIB_STD_FUNCTION_LIKE_TYPE(						\
			VAR_TAG, CV_TAG, REF_TAG, NOEX_TAG, Others						\
		), _VKZLIB_T_ARGS...> &&											\
		std::same_as<														\
			_VKZLIB_T_R,													\
			std::invoke_result_t<VKZLIB_STD_FUNCTION_LIKE_TYPE(				\
				VAR_TAG, CV_TAG, REF_TAG, NOEX_TAG, Others					\
			), _VKZLIB_T_ARGS...>											\
		>																	\
	struct _parse_impl<VKZLIB_STD_FUNCTION_LIKE_TYPE(						\
		VAR_TAG, CV_TAG, REF_TAG, NOEX_TAG, Others							\
	)> : _parse_impl<VKZLIB_FUNCTION_SIGNATURE_TYPE(						\
		VAR_TAG, CV_TAG, REF_TAG, NOEX_TAG									\
	)>																		\
	{																		\
		static constexpr auto _VKZLIB_P_T =									\
			function::parse::type::DirectInvocableType::STLFunctionLike;	\
		using _VKZLIB_N_T = VKZLIB_STD_FUNCTION_LIKE_TYPE(					\
			VAR_TAG,														\
			VKZLIB_PP_SIGNATURE_NONE_TAG,									\
			VKZLIB_PP_SIGNATURE_NONE_TAG,									\
			VKZLIB_PP_SIGNATURE_NONE_TAG,									\
			Others															\
		);																	\
		template<typename S>												\
		using _VKZLIB_F_T = _VKZLIB_T_F<S, Others...>;						\
	};

	VKZLIB_PP_SIGNATURE_MAP_SYNTAX_PRODUCT(VKZLIB_DEFINE_PARSE_STD_FUNCTION_LIKE)

#undef VKZLIB_DEFINE_PARSE_STD_FUNCTION_LIKE
#undef VKZLIB_STD_FUNCTION_LIKE_TYPE

	// ============ Member Function Pointer ============

#define VKZLIB_MEMBER_FUNCTION_POINTER_TYPE(												\
	VAR_TAG, CV_TAG, REF_TAG, NOEX_TAG														\
)																							\
	_VKZLIB_T_R(_VKZLIB_T_C::*)(_VKZLIB_T_ARGS... VKZLIB_PP_SIGNATURE_UNTAG(VAR_TAG))		\
		VKZLIB_PP_SIGNATURE_UNTAG(CV_TAG)													\
		VKZLIB_PP_SIGNATURE_UNTAG(REF_TAG)													\
		VKZLIB_PP_SIGNATURE_UNTAG(NOEX_TAG)

#define VKZLIB_DEFINE_PARSE_MEMBER_FUNCTION_POINTER(								\
	VAR_TAG, CV_TAG, REF_TAG, NOEX_TAG												\
)																					\
	template<																		\
		Class _VKZLIB_T_C,															\
		typename _VKZLIB_T_R, typename ..._VKZLIB_T_ARGS							\
	> struct _parse_impl<VKZLIB_MEMBER_FUNCTION_POINTER_TYPE(						\
		VAR_TAG, CV_TAG, REF_TAG, NOEX_TAG											\
	)> : _parse_impl<VKZLIB_FUNCTION_SIGNATURE_TYPE(								\
		VAR_TAG, CV_TAG, REF_TAG, NOEX_TAG											\
	)>																				\
	{																				\
		static constexpr auto _VKZLIB_P_T =											\
			function::parse::type::IndirectInvocableType::MemberFunctionPointer;	\
		using _VKZLIB_N_T =	VKZLIB_MEMBER_FUNCTION_POINTER_TYPE(					\
			VAR_TAG,																\
			VKZLIB_PP_SIGNATURE_NONE_TAG,											\
			VKZLIB_PP_SIGNATURE_NONE_TAG,											\
			VKZLIB_PP_SIGNATURE_NONE_TAG											\
		);																			\
		using _VKZLIB_C_T = _VKZLIB_T_C;											\
	};

	VKZLIB_PP_SIGNATURE_MAP_SYNTAX_PRODUCT(VKZLIB_DEFINE_PARSE_MEMBER_FUNCTION_POINTER)

#undef VKZLIB_DEFINE_PARSE_MEMBER_FUNCTION_POINTER
#undef VKZLIB_MEMBER_FUNCTION_POINTER_TYPE

#undef VKZLIB_DEFINE_PARSE_FUNCTION_SIGNATURE
#undef VKZLIB_MEMBER_FUNCTION_POINTER_TYPE

	// ============ Monomorphic Functor ============

	template<MonomorphicFunctor L>
	struct _parse_impl<L> : _parse_impl<decltype(&L::operator())> {
		static constexpr auto _VKZLIB_P_T =
			type::DirectInvocableType::MonomorphicFunctor;
		using _VKZLIB_N_T = void;
	};

	// ============ Forward qualified types ============

	template<typename T>
	struct parse : _parse_impl<std::remove_cvref_t<T>> {};

	template<typename T>
		requires (_parse_impl<std::remove_cvref_t<T>>::value)
	struct parse<T> : _parse_impl<std::remove_cvref_t<T>>
	{
		using _VKZLIB_N_T = dup_cvref_t<T, typename _parse_impl<std::remove_cvref_t<T>>::_VKZLIB_N_T>;
	};

#undef VKZLIB_FUNCTION_SIGNATURE_TYPE
#undef VKZLIB_PARSER_TRAIT_SPEC_BODY_COMMON

#undef _VKZLIB_T_PACK
#undef _VKZLIB_T_ARGS
#undef _VKZLIB_T_R
#undef _VKZLIB_T_C
#undef _VKZLIB_T_F
#undef _VKZLIB_T_L
#undef _VKZLIB_NOEX
#undef _VKZLIB_REF
#undef _VKZLIB_CV
#undef _VKZLIB_VAR
#undef _VKZLIB_ARGS_PACK_T
#undef _VKZLIB_R_T
#undef _VKZLIB_N_T
#undef _VKZLIB_F_T
#undef _VKZLIB_C_T
#undef _VKZLIB_P_T

}

#endif // VKZLIB_MPL_FUNCTION_PARSE_PARSE_HPP