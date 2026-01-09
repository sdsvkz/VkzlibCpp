#ifndef VKZLIB_MPL_FUNCTION_H
#define VKZLIB_MPL_FUNCTION_H
#pragma once

#include <concepts>
#include <type_traits>
#include <tuple>

#include "core.hpp"
#include "preprocessor.hpp"

#define VKZLIB_REM_CTOR(...) __VA_ARGS__

namespace vkz {

	namespace mpl {

		namespace function {

			/**
			* @brief Classes with `operator()` but not overloaded
			*/
			template<class T>
			concept MonomorphicFunctionObject = Class<T> && requires {
				{ std::declval<decltype(&T::operator())>() };
			};

			/**
			* @brief Create a normal function signature
			* @tparam isVariadic If it is C variadic function (`...`)
			* @tparam R Return type
			* @tparam ...Args Parameter types
			*/
			template<bool isVariadic, typename R, typename... Args>
			using assemble_signature_t = std::conditional_t<isVariadic,
				R(Args..., ...),
				R(Args...)
			>;

			/**
			* @brief Create a normal member function signature
			* @tparam isVariadic If it is C variadic function (`...`)
			* @tparam R Return type
			* @tparam C Class type
			* @tparam ...Args Parameter types
			*/
			template<bool isVariadic, typename R, Class C, typename... Args>
			using assemble_member_signature_t = std::conditional_t<isVariadic,
				R(C:: *)(Args..., ...),
				R(C:: *)(Args...)
			>;

			// ============ Normalize Traits ============

			// ============ Function Signature ============

			/**
			* @brief Remove cv-qualifiers, noexcept, references, from function signature
			*/
			template<typename T>
			struct normalize_function_signature : std::false_type {};

			// Used to generate all combinations of cv/ref/noexcept/variadic signature specifications
#define _VKZLIB_NORMALIZE_FS_SPEC_IMPL_VAR(VAR_OPT)			\
			VKZLIB_NORMALIZE_TRAIT_SPEC(					\
				(template<typename R, typename ...Args>),	\
				normalize_function_signature,				\
				(R(Args... VKZLIB_REM_CTOR VAR_OPT))		\
			)

			// Applied C variadic parameters:  (none), additional `...` after all types
			// total = 24 * 2 = 48
			_VKZLIB_NORMALIZE_FS_SPEC_IMPL_VAR(());
			_VKZLIB_NORMALIZE_FS_SPEC_IMPL_VAR((, ...));
			/* This comment somehow suppress a false positive intellisense error */
#undef _VKZLIB_NORMALIZE_FS_SPEC_IMPL_VAR

			/**
			 * @brief Function signature, e.g. `void(int, float) noexcept`
			 */
			template <typename S>
			concept Signature = normalize_function_signature<S>::value;


			/**
			* @brief
			* Remove cv-qualifiers, noexcept, references, from function signature
			* 
			* e.g. `void(int, float) const` -> `void(int, float)`
			*/
			template <Signature S>
			using normalize_function_signature_t = normalize_function_signature<S>::type;

			// ============ Function Pointer ============

			/**
			 * @brief Remove cv-qualifiers, noexcept, references, from member function pointer
			 */
			struct normalize_function_pointer : std::false_type {};

			// ============ Member Function Pointer ============

			/**
			* @brief Remove cv-qualifiers, noexcept, references, from member function pointer
			*/
			template <typename T>
			struct normalize_member_function_pointer : std::false_type {};

			// Used to generate all combinations of cv/ref/noexcept/variadic signature specifications
#define _VKZLIB_NORMALIZE_MFP_SPEC_VAR(VAR_OPT) \
			VKZLIB_NORMALIZE_TRAIT_SPEC((template <Class C, typename R, typename... Args>), normalize_member_function_pointer, (R (C::*)(Args... VKZLIB_REM_CTOR VAR_OPT)))

			// Applied C variadic parameters:  (none), additional `...` after all types
			// total = 2 * 4 * 3 * 2 = 48
			_VKZLIB_NORMALIZE_MFP_SPEC_VAR(());
			_VKZLIB_NORMALIZE_MFP_SPEC_VAR((, ...));
			/* This comment somehow suppress a false positive intellisense error */
#undef _VKZLIB_NORMALIZE_MFP_SPEC_VAR

			/**
			* @brief Member function pointer, e.g. `void(Class::*)(int, float) const`
			*/
			template <typename S>
			concept MemberFunctionPointer = normalize_member_function_pointer<S>::value;

			/**
			* @brief
			* Remove cv-qualifiers, noexcept, references, from member function pointer
			* 
			* e.g. `void(Class::*)(int, float) const` -> `void(Class::*)(int, float)`
			*/
			template <MemberFunctionPointer S>
			using normalize_member_function_pointer_t = normalize_member_function_pointer<S>::type;


			/**
			* @brief Default template type used to store template parameter pack
			* @tparam ...Ts Template parameter pack to be stored
			*/
			template <typename ...Ts>
			using DefaultPack = std::tuple<Ts...>;

			// Function signature parsing (Normal)

			template <typename, template<typename...> typename = DefaultPack>
			struct parse_normal_signature : std::false_type {};

			template <template<typename...> typename Pack, typename R, typename ...Args>
			struct parse_normal_signature<R(Args...), Pack> : std::true_type {
				using return_type = R;
				using args_pack_type = Pack<Args...>;
				static constexpr bool with_variadic = false;
			};

			template<template<typename...> typename Pack, typename R, typename ...Args>
			struct parse_normal_signature<R(Args..., ...), Pack> : std::true_type {
				using return_type = R;
				using args_pack_type = Pack<Args...>;
				static constexpr bool with_variadic = true;
			};


			template<typename S, template<typename...> typename Pack = DefaultPack>
			concept NormalSignature = parse_normal_signature<S, Pack>::value;

			template<typename S, template<typename...> typename Pack = DefaultPack>
				requires NormalSignature<S, Pack>
			using normal_sig_result_t = parse_normal_signature<S, Pack>::return_type;

			template<typename S, template<typename...> typename Pack = DefaultPack>
				requires NormalSignature<S, Pack>
			using normal_sig_args_pack_t = parse_normal_signature<S, Pack>::args_pack_type;

			template<typename S, template<typename...> typename Pack = DefaultPack>
			concept VariadicNormalSignature = NormalSignature<S, Pack> && parse_normal_signature<S, Pack>::with_variadic;

			// Function signature parsing (General)

			template <Signature S, template<typename...> typename Pack = DefaultPack>
			using parse_signature = parse_normal_signature<normalize_function_signature_t<S>, Pack>;

			template <Signature S, template<typename...> typename Pack = DefaultPack>
			using sig_result_t = normal_sig_result_t<normalize_function_signature_t<S>, Pack>;

			template <Signature S, template<typename...> typename Pack = DefaultPack>
			using sig_args_pack_t = normal_sig_args_pack_t<normalize_function_signature_t<S>, Pack>;

			template<typename S, template<typename...> typename Pack = DefaultPack>
			concept VariadicSignature =
				Signature<S> &&
				VariadicNormalSignature<normalize_function_signature_t<S>, Pack>;

			// Member signature parsing (Normal)

			template<typename, template<typename...> typename = DefaultPack>
			struct parse_normal_member_signature : std::false_type {};

			template<template<typename...> typename Pack, typename C, typename R, typename ...Args>
			struct parse_normal_member_signature<R(C:: *)(Args...), Pack> : std::true_type {
				using class_type = C;
				using return_type = normal_sig_result_t<R(Args...), Pack>;
				using args_pack_type = normal_sig_args_pack_t<R(Args...), Pack>;
				static constexpr bool with_variadic = false;
			};

			template<template<typename...> typename Pack, typename C, typename R, typename ...Args>
			struct parse_normal_member_signature<R(C:: *)(Args..., ...), Pack> : std::true_type {
				using class_type = C;
				using return_type = normal_sig_result_t<R(Args...), Pack>;
				using args_pack_type = normal_sig_args_pack_t<R(Args...), Pack>;
				static constexpr bool with_variadic = true;
			};


			template<typename S, template<typename...> typename Pack = DefaultPack>
			concept NormalMemberSignature = parse_normal_member_signature<S, Pack>::value;

			template<typename S, template<typename...> typename Pack = DefaultPack>
				requires NormalMemberSignature<S, Pack>
			using normal_member_sig_result_t = parse_normal_member_signature<S, Pack>::return_type;

			template<typename S, template<typename...> typename Pack = DefaultPack>
				requires NormalMemberSignature<S, Pack>
			using normal_member_sig_args_pack_t = parse_normal_member_signature<S, Pack>::args_pack_type;

			template<typename S, template<typename...> typename Pack = DefaultPack>
			concept NormalVariadicMemberSignature =
				NormalMemberSignature<S, Pack> &&
				parse_normal_member_signature<S, Pack>::with_variadic;

			// Member signature parsing (General)

			template<MemberFunctionPointer S, template<typename...> typename Pack = DefaultPack>
			using parse_member_signature = parse_normal_member_signature<normalize_member_function_pointer_t<S>, Pack>;

			template<MemberFunctionPointer S, template<typename...> typename Pack = DefaultPack>
			using member_sig_result_t = normal_member_sig_result_t<normalize_member_function_pointer_t<S>, Pack>;

			template<MemberFunctionPointer S, template<typename...> typename Pack = DefaultPack>
			using member_sig_args_pack_t = normal_member_sig_args_pack_t<normalize_member_function_pointer_t<S>, Pack>;

			template<typename S, template<typename...> typename Pack = DefaultPack>
			concept VariadicMemberSignature =
				MemberFunctionPointer<S> &&
				NormalVariadicMemberSignature<normalize_member_function_pointer_t<S>, Pack>;

			// Parsing non-member function signature from various types

			/**
			 * @brief Parsing non-member function signature from various types
			 */
			template<typename, template<typename...> typename = DefaultPack>
			struct parse_signature_from : std::false_type {};

#define _VKZLIB_PARSE_SIGNATURE_FROM_SPEC_IMPL(SpecPattern, SigPattern, ...)									\
			template<__VA_ARGS__  template<typename...> typename Pack>											\
			struct parse_signature_from<SpecPattern, Pack> : parse_normal_signature<SigPattern, Pack> {};

#define _VKZLIB_PARSE_SIGNATURE_FROM_SPEC(SpecPattern, ...) \
			_VKZLIB_PARSE_SIGNATURE_FROM_SPEC_IMPL(SpecPattern, R(Args...), typename R, typename ...Args, __VA_ARGS__,)

#define _VKZLIB_PARSE_SIGNATURE_FROM_SPEC_VARIADIC(SpecPattern, ...) \
			_VKZLIB_PARSE_SIGNATURE_FROM_SPEC_IMPL(SpecPattern, R(Args..., ...), typename R, typename ...Args, __VA_ARGS__,)

			// TODO:
			// Signature
			_VKZLIB_PARSE_SIGNATURE_FROM_SPEC(R(Args...));
			// Signature (Variadic)
			_VKZLIB_PARSE_SIGNATURE_FROM_SPEC_VARIADIC(R(Args..., ...));
			// Function pointer
			_VKZLIB_PARSE_SIGNATURE_FROM_SPEC(R(*)(Args...));
			// Function pointer (Variadic)
			_VKZLIB_PARSE_SIGNATURE_FROM_SPEC_VARIADIC(R(*)(Args..., ...));
			// std::function like
			_VKZLIB_PARSE_SIGNATURE_FROM_SPEC(F<R(Args...)>, template<typename> typename F);
			// std::function like (Variadic)
			_VKZLIB_PARSE_SIGNATURE_FROM_SPEC_VARIADIC(F<R(Args..., ...)>, template<typename> typename F);

#undef _VKZLIB_PARSE_SIGNATURE_FROM_SPEC_VARIADIC
#undef _VKZLIB_PARSE_SIGNATURE_FROM_SPEC
#undef _VKZLIB_PARSE_SIGNATURE_FROM_SPEC_IMPL

			// Signature
			template <template<typename...> typename Pack, Signature S>
			struct parse_signature_from<S, Pack>
				: parse_signature<S, Pack> {};

			// monomorphic functor type
			template<template<typename...> typename Pack, MonomorphicFunctionObject F>
				requires MemberFunctionPointer<decltype(&F::operator())>
			struct parse_signature_from<F, Pack>
				: parse_member_signature<decltype(&F::operator())> {};

			template<typename F, template<typename...> typename Pack = DefaultPack>
			concept ParsableFuncLike = parse_signature_from<F, Pack>::value;

			template<typename F, template<typename...> typename Pack = DefaultPack>
				requires ParsableFuncLike<F, Pack>
			using result_of_t = parse_signature_from<F, Pack>::return_type;

			template<typename F, template<typename...> typename Pack = DefaultPack>
				requires ParsableFuncLike<F, Pack>
			using args_pack_of_t = parse_signature_from<F, Pack>::args_pack_type;

			template<typename F, template<typename...> typename Pack = DefaultPack>
			concept ParsableVariadicFuncLike = ParsableFuncLike<F, Pack> && parse_signature_from<F, Pack>::with_variadic;

			template<typename F, typename G, template<typename...> typename Pack = DefaultPack>
			concept SameResultAs =
				ParsableFuncLike<F, Pack> &&
				ParsableFuncLike<G, Pack> &&
				std::same_as<result_of_t<F, Pack>, result_of_t<G, Pack>>;

			template<typename F, typename G, template<typename...> typename Pack = DefaultPack>
			concept SameArgsAs =
				ParsableFuncLike<F, Pack> &&
				ParsableFuncLike<G, Pack> &&
				ParsableVariadicFuncLike<F, Pack> == ParsableVariadicFuncLike<G, Pack> &&
				std::same_as<args_pack_of_t<F, Pack>, args_pack_of_t<G, Pack>>;

			template<typename F, typename G, template<typename...> typename Pack = DefaultPack>
			concept Fn =
				SameResultAs<F, G, Pack> &&
				SameArgsAs<F, G, Pack>;

		}

	}

}

#undef VKZLIB_REM_CTOR

#endif // VKZLIB_MPL_FUNCTION_H