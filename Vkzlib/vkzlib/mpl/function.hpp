#ifndef VKZLIB_MPL_FUNCTION_H
#define VKZLIB_MPL_FUNCTION_H
#pragma once

#include <concepts>
#include <type_traits>
#include <tuple>

#include "core.hpp"
#include "preprocessor.hpp"

namespace vkz::mpl::function {

	namespace Type {
		enum class NonInvocable : signed char {
		};

		enum class DirectInvocable : signed char {
			Signature,
			FunctionPointer,
			STLFunctionLike,
			MonomorphicFunctor,
		};

		enum class IndirectInvocable : signed char {
			MemberFunctionPointer,
		};

		template<typename T>
		concept ParsableTypeEnum = AnyOf<T, NonInvocable, DirectInvocable, IndirectInvocable>;
	}

	namespace Property {
		enum class VariadicParam : signed char {
			None,
			Variadic,
		};

		enum class CVQualifier : signed char {
			None,
			Const,
			Volatile,
			ConstVolatile,
		};

		enum class RefQualifier : signed char {
			None,
			LValue,
			RValue,
		};

		enum class ExceptionQualifier : signed char {
			None,
			Noexcept,
		};
	}

	/**
	* @brief Classes with `operator()` but not overloaded
	*/
	template<class T>
	concept MonomorphicFunctor = Class<T> && requires {
		{ std::declval<decltype(&T::operator())>() };
	};

	// ============ Signature parsing ============

	/**
	* @brief Default template type used to store template parameter pack
	* @tparam Ts Template parameter pack to be stored
	*/
	template<typename... Ts>
	using DefaultPack = std::tuple<Ts...>;

	template<typename T>
	struct parse : std::false_type {};

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
		Property::VariadicParam::VKZLIB_PP_SIGNATURE_TAG_TO_ENUM(VAR_TAG);			\
	static constexpr auto _VKZLIB_CV =												\
		Property::CVQualifier::VKZLIB_PP_SIGNATURE_TAG_TO_ENUM(CV_TAG);				\
	static constexpr auto _VKZLIB_REF =												\
		Property::RefQualifier::VKZLIB_PP_SIGNATURE_TAG_TO_ENUM(REF_TAG);			\
	static constexpr auto _VKZLIB_NOEX =											\
		Property::ExceptionQualifier::VKZLIB_PP_SIGNATURE_TAG_TO_ENUM(NOEX_TAG);

#define VKZLIB_DEFINE_COMMON_PARSING_UTILS(												\
	ParserTraitName, ConceptName, NormalTypeHelperName,									\
	ResultTypeHelperName, ArgsPackTypeHelperName,										\
	VariadicHelperName, CVHelperName,													\
	RefHelperName, NoexceptHelperName													\
)																						\
	template<typename T>																\
	concept ConceptName = ParserTraitName<T>::value;									\
																						\
	template<ConceptName T>																\
	using NormalTypeHelperName = ParserTraitName<T>::_VKZLIB_N_T;						\
																						\
	template<ConceptName T>																\
	using ResultTypeHelperName = ParserTraitName<T>::_VKZLIB_R_T;						\
																						\
	template<ConceptName T,																\
		template <typename...> typename Pack = DefaultPack>								\
	using ArgsPackTypeHelperName =														\
		ParserTraitName<T>::template _VKZLIB_ARGS_PACK_T<Pack>;							\
																						\
	template<ConceptName T>																\
	constexpr auto VariadicHelperName = ParserTraitName<T>::_VKZLIB_VAR;				\
																						\
	template<ConceptName T>																\
	constexpr auto CVHelperName = ParserTraitName<T>::_VKZLIB_CV;						\
																						\
	template<ConceptName T>																\
	constexpr auto RefHelperName = ParserTraitName<T>::_VKZLIB_REF;						\
																						\
	template<ConceptName T>																\
	constexpr auto NoexceptHelperName = ParserTraitName<T>::_VKZLIB_NOEX;

	/* Generate specifications:
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

	/* Generate specifications:
	 *	 2. Create a macro that will expand into a specification.
	 *		You can combine `VKZLIB_PP_IF` and `VKZLIB_PP_SIGNATURE_IS_*` macros
	 *		to branch contents based on tags
	 */
#define VKZLIB_DEFINE_PARSE_FUNCTION_SIGNATURE(				\
	VAR_TAG, CV_TAG, REF_TAG, NOEX_TAG						\
)															\
	template<												\
		typename _VKZLIB_T_R, typename ..._VKZLIB_T_ARGS	\
	> struct parse<VKZLIB_FUNCTION_SIGNATURE_TYPE(			\
		VAR_TAG, CV_TAG, REF_TAG, NOEX_TAG					\
	)> : std::true_type										\
	{														\
		static constexpr auto _VKZLIB_P_T =					\
			Type::DirectInvocable::Signature;				\
		using _VKZLIB_N_T = VKZLIB_FUNCTION_SIGNATURE_TYPE(	\
			VAR_TAG,										\
			VKZLIB_PP_SIGNATURE_NONE_TAG,					\
			VKZLIB_PP_SIGNATURE_NONE_TAG,					\
			VKZLIB_PP_SIGNATURE_NONE_TAG					\
		);													\
		VKZLIB_PARSER_TRAIT_SPEC_BODY_COMMON(				\
			VAR_TAG, CV_TAG, REF_TAG, NOEX_TAG				\
		)													\
	};

	/* Generate specifications:
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

#define _VKZLIB_DEFINE_PARSE_FUNCTION_POINTER_IMPL(				\
	VAR_TAG, CV_TAG, REF_TAG, NOEX_TAG							\
)																\
(																\
	template<													\
		typename _VKZLIB_T_R, typename ..._VKZLIB_T_ARGS		\
	> struct parse<VKZLIB_FUNCTION_POINTER_TYPE(				\
		VAR_TAG, CV_TAG, REF_TAG, NOEX_TAG						\
	)> : parse<VKZLIB_FUNCTION_SIGNATURE_TYPE(					\
		VAR_TAG, CV_TAG, REF_TAG, NOEX_TAG						\
	)>															\
	{															\
		static constexpr auto _VKZLIB_P_T =						\
			Type::DirectInvocable::FunctionPointer;				\
		using _VKZLIB_N_T = VKZLIB_FUNCTION_POINTER_TYPE(		\
			VAR_TAG,											\
			VKZLIB_PP_SIGNATURE_NONE_TAG,						\
			VKZLIB_PP_SIGNATURE_NONE_TAG,						\
			VKZLIB_PP_SIGNATURE_NONE_TAG						\
		);														\
	};															\
)

	/* Generate specifications:
	 *	 This is what you can do if you don't want to generate some specifications.
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

#define VKZLIB_DEFINE_PARSE_STD_FUNCTION_LIKE(					\
	VAR_TAG, CV_TAG, REF_TAG, NOEX_TAG							\
)																\
	template<													\
		template <typename, typename...> typename _VKZLIB_T_F,	\
		typename... Others,										\
		typename _VKZLIB_T_R, typename... _VKZLIB_T_ARGS		\
	> requires													\
		std::invocable<VKZLIB_STD_FUNCTION_LIKE_TYPE(			\
			VAR_TAG, CV_TAG, REF_TAG, NOEX_TAG, Others			\
		), _VKZLIB_T_ARGS...> &&								\
		std::same_as<											\
			_VKZLIB_T_R,										\
			std::invoke_result_t<VKZLIB_STD_FUNCTION_LIKE_TYPE(	\
				VAR_TAG, CV_TAG, REF_TAG, NOEX_TAG, Others		\
			), _VKZLIB_T_ARGS...>								\
		>														\
	struct parse<VKZLIB_STD_FUNCTION_LIKE_TYPE(					\
		VAR_TAG, CV_TAG, REF_TAG, NOEX_TAG, Others				\
	)> : parse<VKZLIB_FUNCTION_SIGNATURE_TYPE(					\
		VAR_TAG, CV_TAG, REF_TAG, NOEX_TAG						\
	)>															\
	{															\
		static constexpr auto _VKZLIB_P_T =						\
			Type::DirectInvocable::STLFunctionLike;				\
		using _VKZLIB_N_T = VKZLIB_STD_FUNCTION_LIKE_TYPE(		\
			VAR_TAG,											\
			VKZLIB_PP_SIGNATURE_NONE_TAG,						\
			VKZLIB_PP_SIGNATURE_NONE_TAG,						\
			VKZLIB_PP_SIGNATURE_NONE_TAG,						\
			Others												\
		);														\
		template<typename S>									\
		using _VKZLIB_F_T = _VKZLIB_T_F<S, Others...>;			\
	};

	VKZLIB_PP_SIGNATURE_MAP_SYNTAX_PRODUCT(VKZLIB_DEFINE_PARSE_STD_FUNCTION_LIKE)

	// ============ Member Function Pointer ============

#define VKZLIB_MEMBER_FUNCTION_POINTER_TYPE(												\
	VAR_TAG, CV_TAG, REF_TAG, NOEX_TAG														\
)																							\
	_VKZLIB_T_R(_VKZLIB_T_C::*)(_VKZLIB_T_ARGS... VKZLIB_PP_SIGNATURE_UNTAG(VAR_TAG))		\
		VKZLIB_PP_SIGNATURE_UNTAG(CV_TAG)													\
		VKZLIB_PP_SIGNATURE_UNTAG(REF_TAG)													\
		VKZLIB_PP_SIGNATURE_UNTAG(NOEX_TAG)

#define VKZLIB_DEFINE_PARSE_MEMBER_FUNCTION_POINTER(				\
	VAR_TAG, CV_TAG, REF_TAG, NOEX_TAG								\
)																	\
	template<														\
		Class _VKZLIB_T_C,											\
		typename _VKZLIB_T_R, typename ..._VKZLIB_T_ARGS			\
	> struct parse<VKZLIB_MEMBER_FUNCTION_POINTER_TYPE(				\
		VAR_TAG, CV_TAG, REF_TAG, NOEX_TAG							\
	)> : parse<VKZLIB_FUNCTION_SIGNATURE_TYPE(						\
		VAR_TAG, CV_TAG, REF_TAG, NOEX_TAG							\
	)>																\
	{																\
		static constexpr auto _VKZLIB_P_T =							\
			Type::IndirectInvocable::MemberFunctionPointer;			\
		using _VKZLIB_N_T =	VKZLIB_MEMBER_FUNCTION_POINTER_TYPE(	\
			VAR_TAG,												\
			VKZLIB_PP_SIGNATURE_NONE_TAG,							\
			VKZLIB_PP_SIGNATURE_NONE_TAG,							\
			VKZLIB_PP_SIGNATURE_NONE_TAG							\
		);															\
		using _VKZLIB_C_T = _VKZLIB_T_C;							\
	};

	VKZLIB_PP_SIGNATURE_MAP_SYNTAX_PRODUCT(VKZLIB_DEFINE_PARSE_MEMBER_FUNCTION_POINTER)

#undef VKZLIB_DEFINE_PARSE_FUNCTION_SIGNATURE
#undef VKZLIB_MEMBER_FUNCTION_POINTER_TYPE

	// ============ Monomorphic Functor ============

	template<MonomorphicFunctor L>
	struct parse<L> : parse<decltype(&L::operator())> {
		static constexpr auto _VKZLIB_P_T =
			Type::DirectInvocable::MonomorphicFunctor;
		using _VKZLIB_N_T = void;
	};

	// ============ Forward qualified types ============

	template<typename T>
		requires (!std::same_as<std::remove_cvref_t<T>, T>)
	struct parse<T> : parse<std::remove_cvref_t<T>> {
		using _VKZLIB_N_T = std::enable_if_t<
			parse<std::remove_cvref_t<T>>::value,
			dup_cvref_t<T, typename parse<std::remove_cvref_t<T>>::_VKZLIB_N_T>>;
	};

	// ============ Helpers (Common) ============

	template<typename T>
	concept Parsable = parse<T>::value;

	template<Parsable T>
	constexpr auto type_of = parse<T>::_VKZLIB_P_T;

	template<Parsable T>
	using category_of_t = std::remove_cvref_t<decltype(type_of<T>)>;

	template<typename F, typename G>
	concept SameCategoryAs = std::same_as<category_of_t<F>, category_of_t<G>>;

	template<Type::ParsableTypeEnum C, Type::ParsableTypeEnum E>
	constexpr bool _isAnyTypeOf_impl(C type, E typeEnum) {
		if constexpr (std::same_as<C, E>) {
			return type == typeEnum;
		} else {
			return false;
		}
	}

	template<Parsable T, Type::ParsableTypeEnum... Es>
	constexpr bool isAnyTypeOf(Es... typeEnums) {
		constexpr auto type = type_of<T>;
		return ((_isAnyTypeOf_impl(type, typeEnums)) || ...);
	}

	template<Parsable T>
	using result_of_t = parse<T>::_VKZLIB_R_T;

	template<Parsable T, template <typename...> typename Pack = DefaultPack>
	using args_of_t = parse<T>::template _VKZLIB_ARGS_PACK_T<Pack>;

	template<Parsable T>
	constexpr auto variadic_type_of = parse<T>::_VKZLIB_VAR;

	template<Parsable T>
	constexpr auto cv_qualifier_of = parse<T>::_VKZLIB_CV;

	template<Parsable T>
	constexpr auto ref_qualifier_of = parse<T>::_VKZLIB_REF;

	template<Parsable T>
	constexpr auto exception_qualifier_of = parse<T>::_VKZLIB_NOEX;

	/**
	 * @note Lambda is not normalizable
	 */
	template<typename T>
	concept Normalizable = Parsable<T> &&
		!std::same_as<typename parse<T>::_VKZLIB_N_T, void>;

	/**
	 * @brief Remove CV, Ref, noexcept from signature
	 *
	 * @note Lambda is not normalizable
	 */
	template<Normalizable T>
	using normalize_t = parse<T>::_VKZLIB_N_T;

	// ============ Helpers (Property Requirements) ============

	/**
	 * @brief Property requirements
	 */
	namespace Property::Concepts {

		template<std::equality_comparable T, std::convertible_to<T>... Ts>
		constexpr bool _any_equal(T x, Ts... ys) {
			return ((x == ys) || ...);
		}

		template<typename T>
		concept Variadic = Parsable<T> &&
			variadic_type_of<T> != Property::VariadicParam::None;

		template<typename T>
		concept NonCV = Parsable<T> &&
			cv_qualifier_of<T> == Property::CVQualifier::None;

		template<typename T>
		concept Const = Parsable<T> && _any_equal(cv_qualifier_of<T>,
			Property::CVQualifier::Const,
			Property::CVQualifier::ConstVolatile);

		template<typename T>
		concept Volatile = Parsable<T> && _any_equal(cv_qualifier_of<T>,
			Property::CVQualifier::Volatile,
			Property::CVQualifier::ConstVolatile);

		template<typename T>
		concept NonRef = Parsable<T> &&
			ref_qualifier_of<T> == Property::RefQualifier::None;

		template<typename T>
		concept LValueRef = Parsable<T> &&
			ref_qualifier_of<T> == Property::RefQualifier::LValue;

		template<typename T>
		concept RValueRef = Parsable<T> &&
			ref_qualifier_of<T> == Property::RefQualifier::RValue;

		template<typename T>
		concept NoThrow = Parsable<T> &&
			exception_qualifier_of<T> == Property::ExceptionQualifier::Noexcept;

		template<typename T>
		concept Normal = Parsable<T> &&
			NonCV<T> && NonRef<T> && !NoThrow<T>;
	}

	// ============ Helpers (Concrete Parsable Type) ============

	/**
	 * @brief Concepts for labeling parsable
	 */
	namespace Type::Concepts {
		// Categories

		template<typename T>
		concept NonInvocable = Parsable<T> &&
			std::same_as<category_of_t<T>, enum NonInvocable>;

		template<typename T>
		concept DirectInvocable = Parsable<T> &&
			std::same_as<category_of_t<T>, enum DirectInvocable>;

		template<typename T>
		concept IndirectInvocable = Parsable<T> &&
			std::same_as<category_of_t<T>, enum IndirectInvocable>;

		// Concrete types

		/**
		 * @brief Types like `R(Args...)`
		 */
		template<typename T>
		concept Signature = Parsable<T> &&
			isAnyTypeOf<T>(DirectInvocable::Signature);

		template<typename T>
		concept FunctionPointer = Parsable<T> &&
			isAnyTypeOf<T>(DirectInvocable::FunctionPointer);

		/**
		 * @brief Types similar to `std::function`: First template parameter is a signature
		 */
		template<typename T>
		concept STLFunctionLike = Parsable<T> &&
			isAnyTypeOf<T>(DirectInvocable::STLFunctionLike);

		/**
		 * @brief Types with call operator, but not overloaded
		 */
		template<typename T>
		concept MonomorphicFunctor = Parsable<T> &&
			isAnyTypeOf<T>(DirectInvocable::MonomorphicFunctor);

		template<typename T>
		concept MemberFunctionPointer = Parsable<T> &&
			isAnyTypeOf<T>(IndirectInvocable::MemberFunctionPointer);

	}

	// ============ Helpers (DirectInvocable) ============

	template<Type::Concepts::IndirectInvocable T, typename S>
		requires (isAnyTypeOf<T>(Type::DirectInvocable::STLFunctionLike))
	using function_type_of_t = parse<T>::template _VKZLIB_F_T<S>;

	// ============ Helpers (IndirectInvocable) ============

	template<Type::Concepts::IndirectInvocable T>
		requires (isAnyTypeOf<T>(Type::IndirectInvocable::MemberFunctionPointer))
	using class_of_t = parse<T>::_VKZLIB_C_T;

#undef VKZLIB_FUNCTION_SIGNATURE_TYPE
#undef VKZLIB_DEFINE_COMMON_PARSING_UTILS
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

	// ============ Higher-level utilities ============

	/**
	 * @brief Parsable function type with the same return type as `F`.
	 */
	template<typename F, typename G>
	concept SameResultAs =
		Parsable<F> &&
		Parsable<G> &&
		std::same_as<result_of_t<F>, result_of_t<G>>;

	/**
	 * @brief Parsable function type with the same parameter list as `G`, including C variadic parameter (`...`).
	 *
	 * @tparam Pack Container for storing types of arguments
	 */
	template<typename F, typename G, template <typename...> typename Pack = DefaultPack>
	concept SameArgsAs =
		Parsable<F> &&
		Parsable<G> &&
		Property::Concepts::Variadic<F> == Property::Concepts::Variadic<G> &&
		std::same_as<args_of_t<F, Pack>, args_of_t<G, Pack>>;

	/**
	 * @brief Direct invocable type with same signature as `G`
	 */
	template<typename F, typename G, template <typename...> typename Pack = DefaultPack>
	concept Fn =
		Type::Concepts::DirectInvocable<F> &&
		SameResultAs<F, G> &&
		SameArgsAs<F, G, Pack>;

}

#endif // VKZLIB_MPL_FUNCTION_H