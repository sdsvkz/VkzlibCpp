#include <functional>
#include <memory>
#include <gtest/gtest.h>

#include <vkzlib/mpl.hpp>

#include "common.h"

namespace Test::vkz::mpl {
	using namespace ::vkz::mpl;

	TEST(MplCoreTest, TestAnyOf) {
		using A = int;
		using B = float;
		using C = const char *;
		using D = const int;
		using E = const int &;
		[[maybe_unused]] int i = 1;
		using Int = decltype(i);
		EXPECT_TEMPLATE(TRUE, ::vkz::mpl::AnyOf, Int, A);
		EXPECT_TEMPLATE(TRUE, ::vkz::mpl::AnyOf, Int, A, B, C, D, E);
		EXPECT_TEMPLATE(FALSE, ::vkz::mpl::AnyOf, Int, D);
		EXPECT_TEMPLATE(FALSE, ::vkz::mpl::AnyOf, Int, E);
		EXPECT_TEMPLATE(FALSE, ::vkz::mpl::AnyOf, Int, B, C, D, E);
	}

	TEST(MplCoreTest, TestHomogeneous) {
		using A = int;
		using B = A;
		using C = B;
		using D = int;
		using E = const int;
		using F = const int &;
		EXPECT_TEMPLATE(TRUE, ::vkz::mpl::Homogeneous, A, B, C, D);
		EXPECT_TEMPLATE(FALSE, ::vkz::mpl::Homogeneous, D, E, F);
		EXPECT_TEMPLATE(FALSE, ::vkz::mpl::Homogeneous, E, B, C);
		EXPECT_TEMPLATE(FALSE, ::vkz::mpl:: Homogeneous, B, E, C);
		EXPECT_TEMPLATE(FALSE, ::vkz::mpl:: Homogeneous, B, C, E);
	}

	template<template<typename, typename> typename Dup, typename From, typename To, typename Expect>
	inline constexpr bool _base_same_dup_v = std::same_as<typename Dup<From, To>::type, Expect>;

	template<typename From, typename To, typename Expect>
	inline constexpr bool same_dup_reference_v = _base_same_dup_v<dup_reference, From, To, Expect>;

	TEST(MplCoreTest, TestDupReference) {
#define EXPECT_DUP_REFERENCE_SAME(...) EXPECT_TEMPLATE(TRUE, same_dup_reference_v, __VA_ARGS__)
		EXPECT_DUP_REFERENCE_SAME(int, float, float);
		EXPECT_DUP_REFERENCE_SAME(int, const float, const float);
		EXPECT_DUP_REFERENCE_SAME(int, const volatile float, const volatile float);
		EXPECT_DUP_REFERENCE_SAME(const int, float, float);
		EXPECT_DUP_REFERENCE_SAME(const int, const float, const float);
		EXPECT_DUP_REFERENCE_SAME(const int, volatile float, volatile float);
		EXPECT_DUP_REFERENCE_SAME(volatile int, float, float);
		EXPECT_DUP_REFERENCE_SAME(const volatile int, float, float);
		EXPECT_DUP_REFERENCE_SAME(int &, float, float &);
		EXPECT_DUP_REFERENCE_SAME(int &&, float, float &&);
		EXPECT_DUP_REFERENCE_SAME(const int &, float, float &);
		EXPECT_DUP_REFERENCE_SAME(const int &&, const float, const float &&);
		EXPECT_DUP_REFERENCE_SAME(volatile int &, float, float &);
		EXPECT_DUP_REFERENCE_SAME(volatile int &&, volatile float, volatile float &&);
		EXPECT_DUP_REFERENCE_SAME(const volatile int &&, float, float &&);
		EXPECT_DUP_REFERENCE_SAME(const volatile int &&, const float, const float &&);
		EXPECT_DUP_REFERENCE_SAME(const volatile int &, volatile float, volatile float &);
		EXPECT_DUP_REFERENCE_SAME(const volatile int &, const volatile float, const volatile float &);
		EXPECT_DUP_REFERENCE_SAME(int, float &, float);
		EXPECT_DUP_REFERENCE_SAME(int, float &&, float);
		EXPECT_DUP_REFERENCE_SAME(const int, float &, float);
		EXPECT_DUP_REFERENCE_SAME(const int, const float &&, const float);
		EXPECT_DUP_REFERENCE_SAME(volatile int, float &, float);
		EXPECT_DUP_REFERENCE_SAME(volatile int, volatile float &&, volatile float);
		EXPECT_DUP_REFERENCE_SAME(const volatile int, float &&, float);
		EXPECT_DUP_REFERENCE_SAME(const volatile int, const float &&, const float);
		EXPECT_DUP_REFERENCE_SAME(const volatile int, volatile float &, volatile float);
		EXPECT_DUP_REFERENCE_SAME(const volatile int, const volatile float &, const volatile float);
#undef EXPECT_DUP_REFERENCE_SAME
	}

	template<typename From, typename To, typename Expect>
	constexpr bool same_dup_const_v = _base_same_dup_v<dup_const, From, To, Expect>;

	TEST(MplCoreTest, TestDupConst) {
#define EXPECT_DUP_CONST_SAME(...) EXPECT_TEMPLATE(TRUE, same_dup_const_v, __VA_ARGS__)
		EXPECT_DUP_CONST_SAME(int, float, float);
		EXPECT_DUP_CONST_SAME(int, const float, float);
		EXPECT_DUP_CONST_SAME(int, const volatile float, volatile float);
		EXPECT_DUP_CONST_SAME(const int, float, const float);
		EXPECT_DUP_CONST_SAME(const int, const float, const float);
		EXPECT_DUP_CONST_SAME(const int, volatile float, const volatile float);
		EXPECT_DUP_CONST_SAME(volatile int, float, float);
		EXPECT_DUP_CONST_SAME(const volatile int, float, const float);
		EXPECT_DUP_CONST_SAME(int &, float, float);
		EXPECT_DUP_CONST_SAME(int &&, float, float);
		EXPECT_DUP_CONST_SAME(const int &, float, float);
		EXPECT_DUP_CONST_SAME(const int &&, const float, float);
		EXPECT_DUP_CONST_SAME(volatile int &, float, float);
		EXPECT_DUP_CONST_SAME(volatile int &&, volatile float, volatile float);
		EXPECT_DUP_CONST_SAME(const volatile int &&, float, float);
		EXPECT_DUP_CONST_SAME(const volatile int &&, const float, float);
		EXPECT_DUP_CONST_SAME(const volatile int &, volatile float, volatile float);
		EXPECT_DUP_CONST_SAME(const volatile int &, const volatile float, volatile float);
		EXPECT_DUP_CONST_SAME(int, float &, float &);
		EXPECT_DUP_CONST_SAME(int, float &&, float &&);
		EXPECT_DUP_CONST_SAME(const int, float &, float &);
		EXPECT_DUP_CONST_SAME(const int, const float &&, const float &&);
		EXPECT_DUP_CONST_SAME(volatile int, float &, float &);
		EXPECT_DUP_CONST_SAME(volatile int, volatile float &&, volatile float &&);
		EXPECT_DUP_CONST_SAME(const volatile int, float &&, float &&);
		EXPECT_DUP_CONST_SAME(const volatile int, const float &&, const float &&);
		EXPECT_DUP_CONST_SAME(const volatile int, volatile float &, volatile float &);
		EXPECT_DUP_CONST_SAME(const volatile int, const volatile float &, const volatile float &);
#undef EXPECT_DUP_CONST_SAME
	}

	template<typename From, typename To, typename Expect>
	constexpr bool same_dup_volatile_v = _base_same_dup_v<dup_volatile, From, To, Expect>;

	TEST(MplCoreTest, TestDupVolatile) {
#define EXPECT_DUP_VOLATILE_SAME(...) EXPECT_TEMPLATE(TRUE, same_dup_volatile_v, __VA_ARGS__)
		EXPECT_DUP_VOLATILE_SAME(int, float, float);
		EXPECT_DUP_VOLATILE_SAME(int, const float, const float);
		EXPECT_DUP_VOLATILE_SAME(int, const volatile float, const float);
		EXPECT_DUP_VOLATILE_SAME(const int, float, float);
		EXPECT_DUP_VOLATILE_SAME(const int, const float, const float);
		EXPECT_DUP_VOLATILE_SAME(const int, volatile float, float);
		EXPECT_DUP_VOLATILE_SAME(volatile int, float, volatile float);
		EXPECT_DUP_VOLATILE_SAME(const volatile int, float, volatile float);
		EXPECT_DUP_VOLATILE_SAME(int &, float, float);
		EXPECT_DUP_VOLATILE_SAME(int &&, float, float);
		EXPECT_DUP_VOLATILE_SAME(const int &, float, float);
		EXPECT_DUP_VOLATILE_SAME(const int &&, const float, const float);
		EXPECT_DUP_VOLATILE_SAME(volatile int &, float, float);
		EXPECT_DUP_VOLATILE_SAME(volatile int &&, volatile float, float);
		EXPECT_DUP_VOLATILE_SAME(const volatile int &&, float, float);
		EXPECT_DUP_VOLATILE_SAME(const volatile int &&, const float, const float);
		EXPECT_DUP_VOLATILE_SAME(const volatile int &, volatile float, float);
		EXPECT_DUP_VOLATILE_SAME(const volatile int &, const volatile float, const float);
		EXPECT_DUP_VOLATILE_SAME(int, float &, float &);
		EXPECT_DUP_VOLATILE_SAME(int, float &&, float &&);
		EXPECT_DUP_VOLATILE_SAME(const int, float &, float &);
		EXPECT_DUP_VOLATILE_SAME(const int, const float &&, const float &&);
		EXPECT_DUP_VOLATILE_SAME(volatile int, float &, float &);
		EXPECT_DUP_VOLATILE_SAME(volatile int, volatile float &&, volatile float &&);
		EXPECT_DUP_VOLATILE_SAME(const volatile int, float &&, float &&);
		EXPECT_DUP_VOLATILE_SAME(const volatile int, const float &&, const float &&);
		EXPECT_DUP_VOLATILE_SAME(const volatile int, volatile float &, volatile float &);
		EXPECT_DUP_VOLATILE_SAME(const volatile int, const volatile float &, const volatile float &);
#undef EXPECT_DUP_VOLATILE_SAME
	}

	template<typename From, typename To, typename Expect>
	constexpr bool same_dup_cvref_v = _base_same_dup_v<dup_cvref, From, To, Expect>;

	TEST(MplCoreTest, TestDupCVRef) {
#define EXPECT_DUP_CVREF_SAME(...) EXPECT_TEMPLATE(TRUE, same_dup_cvref_v, __VA_ARGS__)
		EXPECT_DUP_CVREF_SAME(int, float, float);
		EXPECT_DUP_CVREF_SAME(int, const float, float);
		EXPECT_DUP_CVREF_SAME(int, const volatile float, float);
		EXPECT_DUP_CVREF_SAME(const int, float, const float);
		EXPECT_DUP_CVREF_SAME(const int, const float, const float);
		EXPECT_DUP_CVREF_SAME(const int, volatile float, const float);
		EXPECT_DUP_CVREF_SAME(volatile int, float, volatile float);
		EXPECT_DUP_CVREF_SAME(const volatile int, float, const volatile float);
		EXPECT_DUP_CVREF_SAME(int &, float, float &);
		EXPECT_DUP_CVREF_SAME(int &&, float, float &&);
		EXPECT_DUP_CVREF_SAME(const int &, float, const float &);
		EXPECT_DUP_CVREF_SAME(const int &&, const float, const float &&);
		EXPECT_DUP_CVREF_SAME(volatile int &, float, volatile float &);
		EXPECT_DUP_CVREF_SAME(volatile int &&, volatile float, volatile float &&);
		EXPECT_DUP_CVREF_SAME(const volatile int &&, float, const volatile float &&);
		EXPECT_DUP_CVREF_SAME(const volatile int &&, const float, const volatile float &&);
		EXPECT_DUP_CVREF_SAME(const volatile int &, volatile float, const volatile float &);
		EXPECT_DUP_CVREF_SAME(const volatile int &, const volatile float, const volatile float &);
		EXPECT_DUP_CVREF_SAME(int, float &, float);
		EXPECT_DUP_CVREF_SAME(int, float &&, float);
		EXPECT_DUP_CVREF_SAME(const int, float &, const float);
		EXPECT_DUP_CVREF_SAME(const int, const float &&, const float);
		EXPECT_DUP_CVREF_SAME(volatile int, float &, volatile float);
		EXPECT_DUP_CVREF_SAME(volatile int, volatile float &&, volatile float);
		EXPECT_DUP_CVREF_SAME(const volatile int, float &&, const volatile float);
		EXPECT_DUP_CVREF_SAME(const volatile int, const float &&, const volatile float);
		EXPECT_DUP_CVREF_SAME(const volatile int, volatile float &, const volatile float);
		EXPECT_DUP_CVREF_SAME(const volatile int, const volatile float &, const volatile float);
#undef EXPECT_DUP_VOLATILE_SAME
	}

	namespace pack {

		using namespace ::vkz::mpl::pack;

		template<typename Expect, typename N, typename T, typename... Ts>
		constexpr bool nth_is = std::same_as<Expect, nth_of_t<N::value, T, Ts...>>;

		TEST(PackTest, TestNthOf) {
#define EXPECT_NTH_IS(...) EXPECT_TEMPLATE(TRUE, nth_is, __VA_ARGS__)
			EXPECT_NTH_IS(int, std::integral_constant<std::size_t, 0>, int);
			EXPECT_NTH_IS(float, std::integral_constant<std::size_t, 1>, int, float, char, void *);
			EXPECT_NTH_IS(void *, std::integral_constant<std::size_t, 3>, int, float, char, void *);
#undef EXPECT_NTH_IS
		}

		TEST(PackTest, TestPack) {
			using EmptyPack = Pack<>;
			EXPECT_EQ(EmptyPack::size, 0);

			using PackA = Pack<void>;
			EXPECT_EQ(PackA::size, 1);
			EXPECT_TEMPLATE(TRUE, std::same_as, PackA::At<0>, void);

			using PackB = Pack<int, float, char>;
			EXPECT_EQ(PackB::size, 3);
			EXPECT_TEMPLATE(TRUE, std::same_as, PackB::At<0>, int);
			EXPECT_TEMPLATE(TRUE, std::same_as, PackB::At<1>, float);
			EXPECT_TEMPLATE(TRUE, std::same_as, PackB::At<2>, char);
		}
	}

	namespace tpl::fst {

		using namespace ::vkz::mpl::tpl::fst;

		TEST(TplTest, TestIsSameTemplate) {
			constexpr bool e1 = is_same_template_v<std::vector, std::vector>;
			EXPECT_TRUE(e1);
			constexpr bool e2 = is_same_template_v<std::tuple, std::tuple>;
			EXPECT_TRUE(e2);
			constexpr bool e3 = is_same_template_v<std::vector, std::tuple>;
			EXPECT_FALSE(e3);
		}

		TEST(TplTest, TestFirstOrderTemplateSpec) {
			EXPECT_TEMPLATE(TRUE, TemplateSpec, std::vector<int>);
			EXPECT_TEMPLATE(TRUE, TemplateSpec, std::tuple<>);
			EXPECT_TEMPLATE(TRUE, TemplateSpec, std::tuple<int, float>);

			EXPECT_TEMPLATE(FALSE, TemplateSpec, std::true_type);
			EXPECT_TEMPLATE(FALSE, TemplateSpec, std::false_type);
		}

		TEST(TplTest, TestTparamCount) {
			using EmptyTuple = std::tuple<>;
			using TupleA = std::tuple<int, float>;
			using VectorA = std::vector<int>;

			EXPECT_EQ(tparam_count_v<EmptyTuple>, 0);
			EXPECT_EQ(tparam_count_v<TupleA>, 2);
			EXPECT_EQ(tparam_count_v<VectorA>, 2);
		}

	}

	namespace function {

		using namespace ::vkz::mpl::function;

		struct DummyStruct {};

		template<typename S>
		class CallableType;

#define _DEFINE_CALLABLE_TYPE(VAR_TAG, CV_TAG, REF_TAG, NOEX_TAG)					\
(																					\
	template<typename R, typename... Args>											\
	class CallableType<R(Args... VKZLIB_PP_SIGNATURE_UNTAG(VAR_TAG))				\
		VKZLIB_PP_SIGNATURE_UNTAG(CV_TAG)											\
		VKZLIB_PP_SIGNATURE_UNTAG(REF_TAG)											\
		VKZLIB_PP_SIGNATURE_UNTAG(NOEX_TAG)>										\
	{																				\
	public:																			\
		using FunctionType = R(*)(Args... VKZLIB_PP_SIGNATURE_UNTAG(VAR_TAG))		\
			VKZLIB_PP_SIGNATURE_UNTAG(CV_TAG)										\
			VKZLIB_PP_SIGNATURE_UNTAG(REF_TAG)										\
			VKZLIB_PP_SIGNATURE_UNTAG(NOEX_TAG);									\
																					\
		FunctionType f;																\
																					\
		explicit constexpr CallableType(FunctionType f) noexcept : f(f) {};			\
																					\
		R call(Args... args VKZLIB_PP_SIGNATURE_UNTAG(VAR_TAG))						\
			VKZLIB_PP_SIGNATURE_UNTAG(CV_TAG)										\
			VKZLIB_PP_SIGNATURE_UNTAG(REF_TAG)										\
			VKZLIB_PP_SIGNATURE_UNTAG(NOEX_TAG)										\
		{																			\
			return f(args...);														\
		}																			\
																					\
		R operator() (Args... args VKZLIB_PP_SIGNATURE_UNTAG(VAR_TAG))				\
			VKZLIB_PP_SIGNATURE_UNTAG(CV_TAG)										\
			VKZLIB_PP_SIGNATURE_UNTAG(REF_TAG)										\
			VKZLIB_PP_SIGNATURE_UNTAG(NOEX_TAG)										\
		{																			\
			return call(args...);													\
		}																			\
																					\
		using CallOperatorType = POINTER_OF_MEMBER(CallableType, (operator()));		\
		using CallMemberType = POINTER_OF_MEMBER(CallableType, (call));				\
	};																				\
)

#define DEFINE_CALLABLE_TYPE(VAR_TAG, CV_TAG, REF_TAG, NOEX_TAG)	\
	VKZLIB_PP_EXPAND_TUPLE_IF(										\
		VKZLIB_PP_AND(												\
			VKZLIB_PP_SIGNATURE_IS_NONE(CV_TAG),					\
			VKZLIB_PP_SIGNATURE_IS_NONE(REF_TAG)					\
		),															\
		_DEFINE_CALLABLE_TYPE(										\
			VAR_TAG, CV_TAG, REF_TAG, NOEX_TAG						\
		)															\
	)

		VKZLIB_PP_SIGNATURE_MAP_SYNTAX_PRODUCT(DEFINE_CALLABLE_TYPE)

		// template<typename R, typename... Args>
		// class CallableType<R(Args...)>
		// {
		// public:
		// 	using FunctionType = R(*)(Args...);
		//
		// 	FunctionType f;
		//
		// 	template<typename F>
		// 	explicit constexpr CallableType(F f) noexcept : f(f) {};
		//
		// 	R call(Args... args) {
		// 		return f(args...);
		// 	}
		//
		// 	R operator() (Args... args) {
		// 		return call(args...);
		// 	}
		//
		// 	using CallOperatorType = POINTER_OF_MEMBER(CallableType, (operator()));
		// 	using CallMemberType = POINTER_OF_MEMBER(CallableType, (call));
		// };
		//
		// template<typename R, typename... Args>
		// class CallableType<R(Args..., ...)>
		// {
		// public:
		// 	using FunctionType = R(*)(Args..., ...);
		//
		// 	FunctionType f;
		//
		// 	template<typename F>
		// 	explicit constexpr CallableType(F f) noexcept : f(f) {};
		//
		// 	R call(Args... args) {
		// 		return f(args...);
		// 	}
		//
		// 	R operator() (Args... args) {
		// 		return call(args...);
		// 	}
		//
		// 	using CallOperatorType = POINTER_OF_MEMBER(CallableType, (operator()));
		// 	using CallMemberType = POINTER_OF_MEMBER(CallableType, (call));
		// };

		template<typename S, typename Dummy = void>
		class CallableT2;

		template<typename R, typename... Args, typename Dummy>
		class CallableT2<R(Args...), Dummy> {
		public:
			using FunctionType = R(*)(Args...);

			FunctionType f;

			template<typename F>
			explicit constexpr CallableT2(F f) noexcept : f(f) {};

			R call(Args... args) {
				return f(args...);
			}

			R operator() (Args... args) {
				return call(args...);
			}

			using CallOperatorType = POINTER_OF_MEMBER(CallableT2, (operator()));
			using CallMemberType = POINTER_OF_MEMBER(CallableT2, (call));
		};

		template<typename R, typename... Args, typename Dummy>
		class CallableT2<R(Args..., ...), Dummy> {
		public:
			using FunctionType = R(*)(Args..., ...);

			FunctionType f;

			template<typename F>
			explicit constexpr CallableT2(F f) noexcept : f(f) {};

			R call(Args... args) {
				return f(args...);
			}

			R operator() (Args... args) {
				return call(args...);
			}

			using CallOperatorType = POINTER_OF_MEMBER(CallableT2, (operator()));
			using CallMemberType = POINTER_OF_MEMBER(CallableT2, (call));
		};

		template<typename S, typename... Ts>
		class NonCallableT2 {};

		template<typename R, typename... Args, typename... Ts>
		class NonCallableT2<R(Args...), Ts...> {};

		template<typename R, typename... Args, typename... Ts>
		class NonCallableT2<R(Args..., ...), Ts...> {};

		using ExpectedSignature = void(int, float);
		using ExpectedVariadicSignature = void(int, float, ...);

		class DirectInvocableParsingDeps
		{
			using This = DirectInvocableParsingDeps;

		public:
			constexpr DirectInvocableParsingDeps() noexcept = default;

			int iNum { 42 };
			float flNum { 114.514f };
			std::unique_ptr<int> pi { std::make_unique<int>(1919) };

			// Types satisfy `Fn` with expected signature
			using Sig = void(int, float);
			using SigNoexcept = void(int, float) noexcept;
			using ResDiffSig = DummyStruct(int, float);
			using ResDiffVariadicSig = DummyStruct(int, float, ...);
			using NormalPtrFun = void(*)(int, float);
			using PtrFunNoexcept = void(*)(int, float) noexcept;
			using ConstPtrFunNoexcept = void(* const)(int, float) noexcept;
			using NormalRefFun = void(&)(int, float);
			using RefFunNoexcept = void(&)(int, float) noexcept;
			using NormalStdFun = std::function<void(int, float)>;
			using NormalCallable = CallableType<void(int, float)>;
			using RegularLambda = decltype([pi = std::make_unique<int>(42)](int, float) -> void {
				*pi = 1;
			});
			using ConstexprLambda = decltype([](int, float) constexpr noexcept -> void {});
			using MutableLambda = decltype([ptr = std::make_unique<int>(42)](int, float) mutable -> void {
				*ptr = 1;
			});
			// Types not satisfy `Fn` with expected signature
			using ResDiffPtrFun = DummyStruct(*)(int, float);
			using ResDiffLambda = decltype([pi = std::make_unique<int>(42)](int, float) {
				return *pi;
			});
			using ArgsDiffStdFun = std::function<void(int, const float &)>;
			using ArgsDiffCallable = CallableType<void(int, int)>;
			using AllDiffLambda = decltype([pi = std::make_unique<int>(42)](int, const float &) {
				return *pi;
			});
			// Types satisfy `Fn` with expected variadic signature
			using VariadicPtrFun = void(*)(int, float, ...) noexcept;
			using NormalVariadicPtrFun = void(*)(int, float, ...);
			// Types not satisfy `Fn` with expected variadic signature
			using ArgsDiffVariadicPtrFun = void(*)(int, const char *, ...);
			using AllDiffVariadicCallable = CallableType<void *(DummyStruct, float, ...)>;
		};

		class DirectInvocableParsingTest :
			public DirectInvocableParsingDeps,
			public testing::Test {};

		class MFPParsingDepsBase {
			using This = MFPParsingDepsBase;

		public:
			constexpr MFPParsingDepsBase() noexcept = default;
			constexpr virtual ~MFPParsingDepsBase() = default;

			void f(int, float) {}
			void g(int, float) noexcept {}
			void h(int, float) const {}
			void i(int, float) const && {}
			void j(int, float) const & noexcept {}
			virtual void virtualK(int, float) const noexcept {}
			DummyStruct resDiffL(int, float) const && noexcept { return {}; }
			void argsDiffM(int, const DummyStruct &) noexcept {}
			char *allDiffN(const int &, DummyStruct) { return nullptr; }
			void volatileMember(int, float) volatile {}
			void constVolatileMember(int, float) const volatile {}
			void buffedMember(int, float) const volatile && noexcept {}

			template<typename R, typename... Args>
			R variadic(Args... args, ...) const & noexcept {
				return f(args...);
			}

			template<typename R, typename... Args>
			R normalVariadic(Args ...args, ...) {
				return f(args...);
			}

			using CallableCallMember = POINTER_OF_MEMBER(DirectInvocableParsingDeps::NormalCallable, (call));
			// Member function pointers satisfy `Fn` with expected signature
			using NormalMember = POINTER_OF_MEMBER(This, (f));
			using NoexceptMember = POINTER_OF_MEMBER(This, (g));
			using ConstMember = POINTER_OF_MEMBER(This, (h));
			using ConstRRefMember = POINTER_OF_MEMBER(This, (i));
			using ConstLRefNoexceptMember = POINTER_OF_MEMBER(This, (j));
			using VirtualMember = POINTER_OF_MEMBER(This, (virtualK));
			using ResDiffMember = POINTER_OF_MEMBER(This, (resDiffL));
			using ArgsDiffMember = POINTER_OF_MEMBER(This, (argsDiffM));
			using AllDiffMember = POINTER_OF_MEMBER(This, (allDiffN));
			using VolatileMember = POINTER_OF_MEMBER(This, (volatileMember));
			using ConstVolatileMember = POINTER_OF_MEMBER(This, (constVolatileMember));
			using BuffedMember = POINTER_OF_MEMBER(This, (buffedMember));
			template <typename R, typename... Args>
			using VariadicMember = POINTER_OF_MEMBER(This, (variadic<R, Args...>));
			template <typename R, typename... Args>
			using NormalVariadicMember = POINTER_OF_MEMBER(This, (normalVariadic<R, Args...>));
		};

		class MFPParsingDeps : public MFPParsingDepsBase {
			using This = MFPParsingDeps;

		public:
			constexpr MFPParsingDeps() noexcept = default;

			void virtualK(int, float) const noexcept override {}

			using OverrideMember = POINTER_OF_MEMBER(This, (virtualK));
		};

		class MFPParsingTest : public MFPParsingDeps, public testing::Test {};

		class MixedParsingDeps :
			public DirectInvocableParsingDeps,
			public MFPParsingDeps {};

		class MixedParsingTest :
			public MixedParsingDeps,
			public testing::Test {};

		class ParsableConceptDeps :
			public MixedParsingDeps {};

		class ParsableConceptTest :
			public ParsableConceptDeps,
			public testing::Test {};

		class PropertyParsingDeps : public MixedParsingDeps
		{
		public:
			using NormalLambda = decltype([] (int, float) mutable {});
			using NormalOperatorMember = NormalCallable::CallOperatorType;
		};

		class PropertyParsingTest :
			public PropertyParsingDeps,
			public testing::Test {};

		class HigherLevelUtilitiesDeps :
			public MixedParsingDeps {};

		class HigherLevelUtilitiesTest :
			public HigherLevelUtilitiesDeps,
			public testing::Test {};

		namespace parse {
			using namespace ::vkz::mpl::function::parse;

			TEST(NonCallableParsingTest, TestParsable) {
				auto pi = std::make_unique<int>(42);

				EXPECT_TEMPLATE(FALSE, Parsable, int);
				EXPECT_TEMPLATE(FALSE, Parsable, decltype(pi));
			}

			TEST_F(DirectInvocableParsingTest, TestParsable) {
				auto pi = std::make_unique<int>(42);
				using ReferencedLambda = decltype([&pi](int, float) -> void {
					*pi = 1;
				});

				EXPECT_TEMPLATE(TRUE, Parsable, Sig);
				EXPECT_TEMPLATE(TRUE, Parsable, SigNoexcept);
				EXPECT_TEMPLATE(TRUE, Parsable, ResDiffSig);
				EXPECT_TEMPLATE(TRUE, Parsable, ResDiffVariadicSig);
				EXPECT_TEMPLATE(TRUE, Parsable, NormalPtrFun);
				EXPECT_TEMPLATE(TRUE, Parsable, PtrFunNoexcept);
				EXPECT_TEMPLATE(TRUE, Parsable, NormalRefFun);
				EXPECT_TEMPLATE(TRUE, Parsable, RefFunNoexcept);
				EXPECT_TEMPLATE(TRUE, Parsable, NormalStdFun);
				EXPECT_TEMPLATE(TRUE, Parsable, NormalCallable);
				EXPECT_TEMPLATE(TRUE, Parsable, RegularLambda);
				EXPECT_TEMPLATE(TRUE, Parsable, ConstexprLambda);
				EXPECT_TEMPLATE(TRUE, Parsable, MutableLambda);
				EXPECT_TEMPLATE(TRUE, Parsable, ReferencedLambda);
				EXPECT_TEMPLATE(TRUE, Parsable, ResDiffPtrFun);
				EXPECT_TEMPLATE(TRUE, Parsable, ResDiffLambda);
				EXPECT_TEMPLATE(TRUE, Parsable, ArgsDiffStdFun);
				EXPECT_TEMPLATE(TRUE, Parsable, ArgsDiffCallable);
				EXPECT_TEMPLATE(TRUE, Parsable, AllDiffLambda);
				EXPECT_TEMPLATE(TRUE, Parsable, VariadicPtrFun);
				EXPECT_TEMPLATE(TRUE, Parsable, NormalVariadicPtrFun);
				EXPECT_TEMPLATE(TRUE, Parsable, ArgsDiffVariadicPtrFun);
				EXPECT_TEMPLATE(TRUE, Parsable, AllDiffVariadicCallable);
			}

			TEST_F(MFPParsingTest, TestParsable) {
				EXPECT_TEMPLATE(TRUE, Parsable, CallableCallMember);
				EXPECT_TEMPLATE(TRUE, Parsable, NormalMember);
				EXPECT_TEMPLATE(TRUE, Parsable, NoexceptMember);
				EXPECT_TEMPLATE(TRUE, Parsable, ConstMember);
				EXPECT_TEMPLATE(TRUE, Parsable, ConstRRefMember);
				EXPECT_TEMPLATE(TRUE, Parsable, ConstLRefNoexceptMember);
				EXPECT_TEMPLATE(TRUE, Parsable, VirtualMember);
				EXPECT_TEMPLATE(TRUE, Parsable, OverrideMember);
				EXPECT_TEMPLATE(TRUE, Parsable, ResDiffMember);
				EXPECT_TEMPLATE(TRUE, Parsable, ArgsDiffMember);
				EXPECT_TEMPLATE(TRUE, Parsable, AllDiffMember);
				EXPECT_TEMPLATE(TRUE, Parsable, VariadicMember<void, int, float>);
				EXPECT_TEMPLATE(TRUE, Parsable, NormalVariadicMember<DummyStruct, int, float>);
			}

			template<typename Expect, Parsable... Fs>
			constexpr bool all_expected_result = Homogeneous<Expect, result_of_t<Fs>...>;

			TEST_F(DirectInvocableParsingTest, TestResultOf) {
				auto pi = std::make_unique<int>(42);
				using ReferencedLambda = decltype([&pi](int, float) -> void {
					*pi = 1;
				});

				using Expect = void;

				EXPECT_TEMPLATE(TRUE, all_expected_result,
					Expect,
					Sig,
					SigNoexcept,
					NormalPtrFun,
					PtrFunNoexcept,
					NormalRefFun,
					RefFunNoexcept,
					NormalStdFun,
					NormalCallable,
					RegularLambda,
					ConstexprLambda,
					MutableLambda,
					ReferencedLambda,
					VariadicPtrFun,
					NormalVariadicPtrFun,
					ArgsDiffStdFun,
					ArgsDiffCallable,
					ArgsDiffVariadicPtrFun);

				EXPECT_TEMPLATE(FALSE, all_expected_result,
					Sig,
					ResDiffSig);

				EXPECT_TEMPLATE(FALSE, all_expected_result,
					Sig,
					ResDiffVariadicSig);

				EXPECT_TEMPLATE(FALSE, all_expected_result,
					Sig,
					ResDiffLambda);

				EXPECT_TEMPLATE(FALSE, all_expected_result,
					Sig,
					ResDiffPtrFun);

				EXPECT_TEMPLATE(FALSE, all_expected_result,
					Sig,
					AllDiffLambda);

				EXPECT_TEMPLATE(FALSE, all_expected_result,
					Sig,
					AllDiffVariadicCallable);
			}

			TEST_F(MFPParsingTest, TestResultOf) {
				using Expect = void;

				EXPECT_TEMPLATE(TRUE, all_expected_result,
					Expect,
					CallableCallMember,
					NormalMember,
					NoexceptMember,
					ConstMember,
					ConstRRefMember,
					ConstLRefNoexceptMember,
					VirtualMember,
					OverrideMember,
					ArgsDiffMember,
					VariadicMember<void, int, int>,
					NormalVariadicMember<void, int, float>);

				EXPECT_TEMPLATE(FALSE, all_expected_result,
					Expect,
					ResDiffMember);

				EXPECT_TEMPLATE(FALSE, all_expected_result,
					Expect,
					AllDiffMember);

				EXPECT_TEMPLATE(FALSE, all_expected_result,
					Expect,
					VariadicMember<DummyStruct &&, type::DirectInvocableType, type::IndirectInvocableType>);

				EXPECT_TEMPLATE(FALSE, all_expected_result,
					Expect,
					NormalVariadicMember<std::nullptr_t, int, float>);
			}

			TEST_F(MixedParsingTest, TestResultOf) {
				auto pi = std::make_unique<int>(42);
				using ReferencedLambda = decltype([&pi](int, float) -> void {
					*pi = 1;
				});

				using Expect = void;

				EXPECT_TEMPLATE(TRUE, all_expected_result,
					Expect,
					Sig,
					SigNoexcept,
					NormalPtrFun,
					PtrFunNoexcept,
					NormalRefFun,
					RefFunNoexcept,
					NormalStdFun,
					NormalCallable,
					RegularLambda,
					ConstexprLambda,
					MutableLambda,
					ReferencedLambda,
					VariadicPtrFun,
					NormalVariadicPtrFun,
					ArgsDiffStdFun,
					ArgsDiffCallable,
					ArgsDiffVariadicPtrFun,
					CallableCallMember,
					NormalMember,
					NoexceptMember,
					ConstMember,
					ConstRRefMember,
					ConstLRefNoexceptMember,
					VirtualMember,
					OverrideMember,
					ArgsDiffMember,
					VariadicMember<void, int, int>,
					NormalVariadicMember<void, int, float>);
			}

			template<typename Expect, Parsable... Fs>
			constexpr bool all_expected_args = Homogeneous<Expect, args_of_t<Fs>...>;

			TEST_F(DirectInvocableParsingTest, TestArgsOf) {
				auto pi = std::make_unique<int>(42);
				using ReferencedLambda = decltype([&pi](int, float) -> void {
					*pi = 1;
				});

				using Expect = DefaultPack<int, float>;

				EXPECT_TEMPLATE(TRUE, all_expected_args,
					Expect,
					Sig,
					SigNoexcept,
					ResDiffSig,
					ResDiffVariadicSig,
					NormalPtrFun,
					PtrFunNoexcept,
					NormalStdFun,
					NormalCallable,
					RegularLambda,
					ConstexprLambda,
					MutableLambda,
					ReferencedLambda,
					ResDiffPtrFun,
					ResDiffLambda);

				EXPECT_TEMPLATE(FALSE, all_expected_args,
					Expect,
					ArgsDiffStdFun);

				EXPECT_TEMPLATE(FALSE, all_expected_args,
					Expect,
					ArgsDiffCallable);

				EXPECT_TEMPLATE(FALSE, all_expected_args,
					Expect,
					ArgsDiffVariadicPtrFun);
			}

			TEST_F(MFPParsingTest, TestArgsOf) {
				using Expect = DefaultPack<int, float>;

				EXPECT_TEMPLATE(TRUE, all_expected_args,
					Expect,
					CallableCallMember,
					NormalMember,
					NoexceptMember,
					ConstMember,
					ConstRRefMember,
					ConstLRefNoexceptMember,
					VirtualMember,
					OverrideMember,
					ResDiffMember,
					VariadicMember<const DummyStruct &, int, float>,
					NormalVariadicMember<void, int, float>);

				EXPECT_TEMPLATE(FALSE, all_expected_args,
					Expect,
					ArgsDiffMember);

				EXPECT_TEMPLATE(FALSE, all_expected_args,
					Expect,
					AllDiffMember);

				EXPECT_TEMPLATE(FALSE, all_expected_args,
					Expect,
					VariadicMember<const DummyStruct &, int &&, float **>);

				EXPECT_TEMPLATE(FALSE, all_expected_args,
					Expect,
					NormalVariadicMember<void, float, float>);
			}

			TEST_F(MixedParsingTest, TestArgsOf) {
				auto pi = std::make_unique<int>(42);
				using ReferencedLambda = decltype([&pi](int, float) -> void {
					*pi = 1;
				});

				using Expect = DefaultPack<int, float>;

				EXPECT_TEMPLATE(TRUE, all_expected_args,
					Expect,
					Sig,
					SigNoexcept,
					ResDiffSig,
					ResDiffVariadicSig,
					NormalPtrFun,
					PtrFunNoexcept,
					NormalStdFun,
					NormalCallable,
					RegularLambda,
					ConstexprLambda,
					MutableLambda,
					ReferencedLambda,
					ResDiffPtrFun,
					ResDiffLambda,
					CallableCallMember,
					NormalMember,
					NoexceptMember,
					ConstMember,
					ConstRRefMember,
					ConstLRefNoexceptMember,
					VirtualMember,
					OverrideMember,
					ResDiffMember,
					VariadicMember<const DummyStruct &, int, float>,
					NormalVariadicMember<void, int, float>);
			}

			template<typename Normal, typename What>
			constexpr bool same_as_normalized_v = std::same_as<Normal, normalize_t<What>>;

			TEST_F(DirectInvocableParsingTest, TestNormalize) {
				using ConstLRefStdFun = const std::function<void(int, float)> &;
				using NoexceptCallable = CallableType<void(int, float) noexcept>;

				EXPECT_TEMPLATE(TRUE, same_as_normalized_v, Sig, Sig);
				EXPECT_TEMPLATE(TRUE, same_as_normalized_v, Sig, SigNoexcept);
				EXPECT_TEMPLATE(TRUE, same_as_normalized_v, NormalPtrFun, NormalPtrFun);
				EXPECT_TEMPLATE(TRUE, same_as_normalized_v, NormalPtrFun, PtrFunNoexcept);
				EXPECT_TEMPLATE(TRUE, same_as_normalized_v, NormalRefFun, NormalRefFun);
				EXPECT_TEMPLATE(TRUE, same_as_normalized_v, NormalRefFun, RefFunNoexcept);
				EXPECT_TEMPLATE(TRUE, same_as_normalized_v, NormalStdFun, NormalStdFun);
				EXPECT_TEMPLATE(TRUE, same_as_normalized_v, ConstLRefStdFun, ConstLRefStdFun);
				EXPECT_TEMPLATE(TRUE, same_as_normalized_v, NormalCallable, NormalCallable);
				EXPECT_TEMPLATE(TRUE, same_as_normalized_v, NormalCallable, NoexceptCallable);
				EXPECT_TEMPLATE(TRUE, same_as_normalized_v, NormalVariadicPtrFun, NormalVariadicPtrFun);
				EXPECT_TEMPLATE(TRUE, same_as_normalized_v, NormalVariadicPtrFun, VariadicPtrFun);

				EXPECT_TEMPLATE(FALSE, same_as_normalized_v, NormalPtrFun, ResDiffPtrFun);
				EXPECT_TEMPLATE(FALSE, same_as_normalized_v, NormalCallable, ArgsDiffCallable);
				EXPECT_TEMPLATE(FALSE, same_as_normalized_v, NormalPtrFun, NormalVariadicPtrFun);
			}

			TEST_F(MFPParsingTest, TestNormalize) {
				EXPECT_TEMPLATE(TRUE, same_as_normalized_v, NormalMember, NormalMember);
				EXPECT_TEMPLATE(TRUE, same_as_normalized_v, NormalMember, BuffedMember);
				EXPECT_TEMPLATE(TRUE, same_as_normalized_v, NormalMember, ConstLRefNoexceptMember);
				EXPECT_TEMPLATE(TRUE, same_as_normalized_v, NormalMember, ConstMember);
				EXPECT_TEMPLATE(TRUE, same_as_normalized_v, NormalMember, ConstRRefMember);
				EXPECT_TEMPLATE(TRUE, same_as_normalized_v, NormalMember, ConstVolatileMember);
				EXPECT_TEMPLATE(TRUE, same_as_normalized_v, NormalMember, NoexceptMember);
				EXPECT_TEMPLATE(TRUE, same_as_normalized_v, NormalMember, VolatileMember);
				EXPECT_TEMPLATE(TRUE, same_as_normalized_v, NormalVariadicMember<void, int, float>, NormalVariadicMember<void, int, float>);
				EXPECT_TEMPLATE(TRUE, same_as_normalized_v, NormalVariadicMember<void, int, float>, VariadicMember<void, int, float>);

				EXPECT_TEMPLATE(FALSE, same_as_normalized_v, NormalMember, ResDiffMember);
				EXPECT_TEMPLATE(FALSE, same_as_normalized_v, NormalMember, ArgsDiffMember);
				EXPECT_TEMPLATE(FALSE, same_as_normalized_v, NormalMember, NormalVariadicMember<void, int, float>);
			}

			TEST_F(MixedParsingTest, TestNormalize) {
				EXPECT_TEMPLATE(FALSE, same_as_normalized_v, NormalPtrFun, NormalMember);
				EXPECT_TEMPLATE(FALSE, same_as_normalized_v, NormalMember, NormalPtrFun);
			}

			namespace property {
				using namespace ::vkz::mpl::function::parse::property;

				TEST_F(PropertyParsingTest, TestVariadic) {
#define EXPECT_VARIADIC(BOOL, ...) EXPECT_TEMPLATE(BOOL, Variadic, __VA_ARGS__)
					EXPECT_VARIADIC(TRUE, NormalVariadicPtrFun);
					EXPECT_VARIADIC(TRUE, ResDiffVariadicSig);
					EXPECT_VARIADIC(TRUE, ArgsDiffVariadicPtrFun);
					EXPECT_VARIADIC(TRUE, AllDiffVariadicCallable);
					EXPECT_VARIADIC(TRUE, VariadicMember<void, int, float>);

					EXPECT_VARIADIC(FALSE, NormalPtrFun);
					EXPECT_VARIADIC(FALSE, SigNoexcept);
					EXPECT_VARIADIC(FALSE, AllDiffLambda);
					EXPECT_VARIADIC(FALSE, NormalMember);
					EXPECT_VARIADIC(FALSE, CallableCallMember);

					EXPECT_VARIADIC(FALSE, void);
					EXPECT_VARIADIC(FALSE, int);
				}

				TEST_F(PropertyParsingTest, TestNonCV) {
#define EXPECT_NONCV(BOOL, ...) EXPECT_TEMPLATE(BOOL, NonCV, __VA_ARGS__)
					EXPECT_NONCV(TRUE, NormalPtrFun);
					EXPECT_NONCV(TRUE, NormalRefFun);
					EXPECT_NONCV(TRUE, NormalStdFun);
					EXPECT_NONCV(TRUE, NormalCallable);
					EXPECT_NONCV(TRUE, NormalLambda);
					EXPECT_NONCV(TRUE, NormalVariadicPtrFun);
					EXPECT_NONCV(TRUE, NormalMember);
					EXPECT_NONCV(TRUE, NormalOperatorMember);
					EXPECT_NONCV(TRUE, SigNoexcept);
					EXPECT_NONCV(TRUE, PtrFunNoexcept);
					EXPECT_NONCV(TRUE, RefFunNoexcept);
					EXPECT_NONCV(TRUE, NoexceptMember);

					EXPECT_NONCV(FALSE, ConstLRefNoexceptMember);
					EXPECT_NONCV(FALSE, VariadicMember<void, int, float>);

					EXPECT_NONCV(FALSE, void);
					EXPECT_NONCV(FALSE, int);
#undef EXPECT_NONCV
				}

				TEST_F(PropertyParsingTest, TestConst) {
#define EXPECT_CONST(BOOL, ...) EXPECT_TEMPLATE(BOOL, Const, __VA_ARGS__)
					EXPECT_CONST(TRUE, RegularLambda);
					EXPECT_CONST(TRUE, ConstexprLambda);
					EXPECT_CONST(TRUE, ConstMember);
					EXPECT_CONST(TRUE, ConstLRefNoexceptMember);
					EXPECT_CONST(TRUE, ConstRRefMember);

					EXPECT_CONST(FALSE, NormalPtrFun);
					EXPECT_CONST(FALSE, NormalRefFun);
					EXPECT_CONST(FALSE, NormalStdFun);
					EXPECT_CONST(FALSE, NormalCallable);
					EXPECT_CONST(FALSE, NormalLambda);
					EXPECT_CONST(FALSE, NormalVariadicPtrFun);
					EXPECT_CONST(FALSE, NormalMember);
					EXPECT_CONST(FALSE, NormalOperatorMember);
					EXPECT_CONST(FALSE, Sig);
					EXPECT_CONST(FALSE, SigNoexcept);
					EXPECT_CONST(FALSE, PtrFunNoexcept);
					EXPECT_CONST(FALSE, RefFunNoexcept);
					EXPECT_CONST(FALSE, MutableLambda);

					EXPECT_CONST(FALSE, void);
					EXPECT_CONST(FALSE, int);
#undef EXPECT_CONST
				}

				TEST_F(PropertyParsingTest, TestVolatile) {
#define EXPECT_VOLATILE(BOOL, ...) EXPECT_TEMPLATE(BOOL, Volatile, __VA_ARGS__)
					EXPECT_VOLATILE(TRUE, VolatileMember);
					EXPECT_VOLATILE(TRUE, ConstVolatileMember);
					EXPECT_VOLATILE(TRUE, BuffedMember);

					EXPECT_VOLATILE(FALSE, RegularLambda);
					EXPECT_VOLATILE(FALSE, ConstexprLambda);
					EXPECT_VOLATILE(FALSE, ConstMember);
					EXPECT_VOLATILE(FALSE, ConstLRefNoexceptMember);
					EXPECT_VOLATILE(FALSE, ConstRRefMember);

					EXPECT_VOLATILE(FALSE, NormalPtrFun);
					EXPECT_VOLATILE(FALSE, NormalRefFun);
					EXPECT_VOLATILE(FALSE, NormalStdFun);
					EXPECT_VOLATILE(FALSE, NormalCallable);
					EXPECT_VOLATILE(FALSE, NormalLambda);
					EXPECT_VOLATILE(FALSE, NormalVariadicPtrFun);
					EXPECT_VOLATILE(FALSE, NormalMember);
					EXPECT_VOLATILE(FALSE, NormalOperatorMember);
					EXPECT_VOLATILE(FALSE, Sig);
					EXPECT_VOLATILE(FALSE, SigNoexcept);
					EXPECT_VOLATILE(FALSE, PtrFunNoexcept);
					EXPECT_VOLATILE(FALSE, RefFunNoexcept);
					EXPECT_VOLATILE(FALSE, MutableLambda);

					EXPECT_VOLATILE(FALSE, void);
					EXPECT_VOLATILE(FALSE, int);
#undef EXPECT_VOLATILE
				}

				template<typename T>
				concept ConstVolatile = Const<T> && Volatile<T>;

				TEST_F(PropertyParsingTest, TestConstVolatile) {
#define EXPECT_CONST_VOLATILE(BOOL, ...) EXPECT_TEMPLATE(BOOL, ConstVolatile, __VA_ARGS__)
					EXPECT_CONST_VOLATILE(TRUE, ConstVolatileMember);
					EXPECT_CONST_VOLATILE(TRUE, BuffedMember);

					EXPECT_CONST_VOLATILE(FALSE, RegularLambda);
					EXPECT_CONST_VOLATILE(FALSE, ConstexprLambda);
					EXPECT_CONST_VOLATILE(FALSE, ConstMember);
					EXPECT_CONST_VOLATILE(FALSE, ConstLRefNoexceptMember);
					EXPECT_CONST_VOLATILE(FALSE, ConstRRefMember);
					EXPECT_CONST_VOLATILE(FALSE, VolatileMember);

					EXPECT_CONST_VOLATILE(FALSE, NormalPtrFun);
					EXPECT_CONST_VOLATILE(FALSE, NormalRefFun);
					EXPECT_CONST_VOLATILE(FALSE, NormalStdFun);
					EXPECT_CONST_VOLATILE(FALSE, NormalCallable);
					EXPECT_CONST_VOLATILE(FALSE, NormalLambda);
					EXPECT_CONST_VOLATILE(FALSE, NormalVariadicPtrFun);
					EXPECT_CONST_VOLATILE(FALSE, NormalMember);
					EXPECT_CONST_VOLATILE(FALSE, NormalOperatorMember);
					EXPECT_CONST_VOLATILE(FALSE, Sig);
					EXPECT_CONST_VOLATILE(FALSE, SigNoexcept);
					EXPECT_CONST_VOLATILE(FALSE, PtrFunNoexcept);
					EXPECT_CONST_VOLATILE(FALSE, RefFunNoexcept);
					EXPECT_CONST_VOLATILE(FALSE, MutableLambda);

					EXPECT_CONST_VOLATILE(FALSE, void);
					EXPECT_CONST_VOLATILE(FALSE, int);
#undef EXPECT_CONST_VOLATILE
				}

				TEST_F(PropertyParsingTest, TestNonRef) {
#define EXPECT_NONREF(BOOL, ...) EXPECT_TEMPLATE(BOOL, NonRef, __VA_ARGS__)
					EXPECT_NONREF(TRUE, NormalPtrFun);
					EXPECT_NONREF(TRUE, NormalRefFun);
					EXPECT_NONREF(TRUE, NormalStdFun);
					EXPECT_NONREF(TRUE, NormalCallable);
					EXPECT_NONREF(TRUE, NormalLambda);
					EXPECT_NONREF(TRUE, NormalVariadicPtrFun);
					EXPECT_NONREF(TRUE, NormalMember);
					EXPECT_NONREF(TRUE, NormalOperatorMember);

					EXPECT_NONREF(TRUE, SigNoexcept);
					EXPECT_NONREF(TRUE, PtrFunNoexcept);
					EXPECT_NONREF(TRUE, RefFunNoexcept);
					EXPECT_NONREF(TRUE, NoexceptMember);

					EXPECT_NONREF(FALSE, ConstLRefNoexceptMember);
					EXPECT_NONREF(FALSE, ConstRRefMember);
					EXPECT_NONREF(FALSE, ResDiffMember);
					EXPECT_NONREF(FALSE, BuffedMember);
					EXPECT_NONREF(FALSE, VariadicMember<void, int, float>);

					EXPECT_NONREF(FALSE, int);
					EXPECT_NONREF(FALSE, float);
#undef EXPECT_NONREF
				}

				TEST_F(PropertyParsingTest, TestLValueRef) {
#define  EXPECT_LVALUEREF(BOOL, ...) EXPECT_TEMPLATE(BOOL, LValueRef, __VA_ARGS__)
					EXPECT_LVALUEREF(TRUE, ConstLRefNoexceptMember);
					EXPECT_LVALUEREF(TRUE, VariadicMember<void, int, float>);

					EXPECT_LVALUEREF(FALSE, NormalPtrFun);
					EXPECT_LVALUEREF(FALSE, NormalRefFun);
					EXPECT_LVALUEREF(FALSE, NormalStdFun);
					EXPECT_LVALUEREF(FALSE, NormalCallable);
					EXPECT_LVALUEREF(FALSE, NormalLambda);
					EXPECT_LVALUEREF(FALSE, NormalVariadicPtrFun);
					EXPECT_LVALUEREF(FALSE, NormalMember);
					EXPECT_LVALUEREF(FALSE, NormalOperatorMember);

					EXPECT_LVALUEREF(FALSE, SigNoexcept);
					EXPECT_LVALUEREF(FALSE, PtrFunNoexcept);
					EXPECT_LVALUEREF(FALSE, RefFunNoexcept);
					EXPECT_LVALUEREF(FALSE, NoexceptMember);

					EXPECT_LVALUEREF(FALSE, ConstRRefMember);
					EXPECT_LVALUEREF(FALSE, ResDiffMember);
					EXPECT_LVALUEREF(FALSE, BuffedMember);

					EXPECT_LVALUEREF(FALSE, int);
					EXPECT_LVALUEREF(FALSE, float);
#undef EXPECT_LVALUEREF
				}

				TEST_F(PropertyParsingTest, TestRValueRef) {
#define EXPECT_RVALUEREF(BOOL, ...) EXPECT_TEMPLATE(BOOL, RValueRef, __VA_ARGS__)
					EXPECT_RVALUEREF(TRUE, ConstRRefMember);
					EXPECT_RVALUEREF(TRUE, ResDiffMember);
					EXPECT_RVALUEREF(TRUE, BuffedMember);

					EXPECT_RVALUEREF(FALSE, ConstLRefNoexceptMember);
					EXPECT_RVALUEREF(FALSE, VariadicMember<void, int, float>);

					EXPECT_RVALUEREF(FALSE, NormalPtrFun);
					EXPECT_RVALUEREF(FALSE, NormalRefFun);
					EXPECT_RVALUEREF(FALSE, NormalStdFun);
					EXPECT_RVALUEREF(FALSE, NormalCallable);
					EXPECT_RVALUEREF(FALSE, NormalLambda);
					EXPECT_RVALUEREF(FALSE, NormalVariadicPtrFun);
					EXPECT_RVALUEREF(FALSE, NormalMember);
					EXPECT_RVALUEREF(FALSE, NormalOperatorMember);

					EXPECT_RVALUEREF(FALSE, SigNoexcept);
					EXPECT_RVALUEREF(FALSE, PtrFunNoexcept);
					EXPECT_RVALUEREF(FALSE, RefFunNoexcept);
					EXPECT_RVALUEREF(FALSE, NoexceptMember);

					EXPECT_RVALUEREF(FALSE, int);
					EXPECT_RVALUEREF(FALSE, float);
#undef EXPECT_RVALUEREF
				}

				TEST_F(PropertyParsingTest, TestNoThrow) {
#define EXPECT_NOTHROW(BOOL, ...) EXPECT_TEMPLATE(BOOL, NoThrow, __VA_ARGS__)
					EXPECT_NOTHROW(TRUE, SigNoexcept);
					EXPECT_NOTHROW(TRUE, PtrFunNoexcept);
					EXPECT_NOTHROW(TRUE, RefFunNoexcept);
					EXPECT_NOTHROW(TRUE, NoexceptMember);
					EXPECT_NOTHROW(TRUE, ResDiffMember);
					EXPECT_NOTHROW(TRUE, ConstLRefNoexceptMember);
					EXPECT_NOTHROW(TRUE, BuffedMember);
					EXPECT_NOTHROW(TRUE, VariadicMember<void, int, float>);


					EXPECT_NOTHROW(FALSE, NormalPtrFun);
					EXPECT_NOTHROW(FALSE, NormalRefFun);
					EXPECT_NOTHROW(FALSE, NormalStdFun);
					EXPECT_NOTHROW(FALSE, NormalCallable);
					EXPECT_NOTHROW(FALSE, NormalLambda);
					EXPECT_NOTHROW(FALSE, NormalVariadicPtrFun);
					EXPECT_NOTHROW(FALSE, NormalMember);
					EXPECT_NOTHROW(FALSE, NormalOperatorMember);

					EXPECT_NOTHROW(FALSE, ConstRRefMember);

					EXPECT_NOTHROW(FALSE, int);
					EXPECT_NOTHROW(FALSE, float);
#undef EXPECT_NOTHROW
				}

				TEST_F(PropertyParsingTest, TestNormal) {
#define EXPECT_NORMAL(BOOL, ...) EXPECT_TEMPLATE(BOOL, Normal, __VA_ARGS__)
					EXPECT_NORMAL(TRUE, NormalPtrFun);
					EXPECT_NORMAL(TRUE, NormalRefFun);
					EXPECT_NORMAL(TRUE, NormalStdFun);
					EXPECT_NORMAL(TRUE, NormalCallable);
					EXPECT_NORMAL(TRUE, NormalLambda);
					EXPECT_NORMAL(TRUE, NormalVariadicPtrFun);
					EXPECT_NORMAL(TRUE, NormalMember);
					EXPECT_NORMAL(TRUE, NormalOperatorMember);

					EXPECT_NORMAL(FALSE, SigNoexcept);
					EXPECT_NORMAL(FALSE, PtrFunNoexcept);
					EXPECT_NORMAL(FALSE, RefFunNoexcept);
					EXPECT_NORMAL(FALSE, NoexceptMember);
					EXPECT_NORMAL(FALSE, ConstLRefNoexceptMember);
					EXPECT_NORMAL(FALSE, VariadicMember<void, int, float>);

					EXPECT_NORMAL(FALSE, void);
					EXPECT_NORMAL(FALSE, int);
#undef EXPECT_NORMAL
				}
			}

			namespace type {
				using namespace ::vkz::mpl::function::parse::type;

				TEST_F(ParsableConceptTest, TestNonInvocable) {
#define EXPECT_NI(BOOL, ...) EXPECT_TEMPLATE(BOOL, NonInvocable, __VA_ARGS__)
					EXPECT_NI(FALSE, ExpectedSignature);
					EXPECT_NI(FALSE, SigNoexcept);
					EXPECT_NI(FALSE, ResDiffSig);
					EXPECT_NI(FALSE, ResDiffVariadicSig);
					EXPECT_NI(FALSE, ExpectedVariadicSignature);
					EXPECT_NI(FALSE, NormalRefFun);
					EXPECT_NI(FALSE, RefFunNoexcept);
					EXPECT_NI(FALSE, NormalPtrFun);
					EXPECT_NI(FALSE, PtrFunNoexcept);
					EXPECT_NI(FALSE, NormalStdFun);
					EXPECT_NI(FALSE, NormalCallable);
					EXPECT_NI(FALSE, RegularLambda);
					EXPECT_NI(FALSE, NormalMember);
					EXPECT_NI(FALSE, NormalVariadicPtrFun);
#undef EXPECT_NI
				}

				TEST_F(ParsableConceptTest, TestDirectInvocable) {
#define EXPECT_DI(BOOL, ...) EXPECT_TEMPLATE(BOOL, DirectInvocable, __VA_ARGS__)
					EXPECT_DI(TRUE, SigNoexcept);
					EXPECT_DI(TRUE, NormalRefFun);
					EXPECT_DI(TRUE, RefFunNoexcept);
					EXPECT_DI(TRUE, NormalPtrFun);
					EXPECT_DI(TRUE, PtrFunNoexcept);
					EXPECT_DI(TRUE, NormalStdFun);
					EXPECT_DI(TRUE, NormalCallable);
					EXPECT_DI(TRUE, RegularLambda);
					EXPECT_DI(TRUE, ConstexprLambda);
					EXPECT_DI(TRUE, MutableLambda);
					EXPECT_DI(TRUE, VariadicPtrFun);

					EXPECT_DI(FALSE, CallableCallMember);
					EXPECT_DI(FALSE, NormalMember);
					EXPECT_DI(FALSE, NoexceptMember);
					EXPECT_DI(FALSE, ConstMember);
					EXPECT_DI(FALSE, ConstRRefMember);
					EXPECT_DI(FALSE, ConstLRefNoexceptMember);
					EXPECT_DI(FALSE, VirtualMember);
					EXPECT_DI(FALSE, OverrideMember);
#undef EXPECT_DI
				}

				TEST_F(ParsableConceptTest, TestSignature) {
#define EXPECT_SIG(BOOL, ...) EXPECT_TEMPLATE(BOOL, Signature, __VA_ARGS__)
					EXPECT_SIG(TRUE, ExpectedSignature);
					EXPECT_SIG(TRUE, Sig);
					EXPECT_SIG(TRUE, SigNoexcept);
					EXPECT_SIG(TRUE, ResDiffSig);
					EXPECT_SIG(TRUE, ResDiffVariadicSig);
					EXPECT_SIG(TRUE, ExpectedVariadicSignature);
					// Reference to `void(int, float)`
					EXPECT_SIG(TRUE, NormalRefFun);
					EXPECT_SIG(TRUE, RefFunNoexcept);

					EXPECT_SIG(FALSE, NormalPtrFun);
					EXPECT_SIG(FALSE, PtrFunNoexcept);
					EXPECT_SIG(FALSE, NormalStdFun);
					EXPECT_SIG(FALSE, NormalCallable);
					EXPECT_SIG(FALSE, RegularLambda);
					EXPECT_SIG(FALSE, NormalMember);
					EXPECT_SIG(FALSE, NormalVariadicPtrFun);
#undef EXPECT_SIG
				}

				TEST_F(ParsableConceptTest, TestFunctionPointer) {
#define EXPECT_FP(BOOL, ...) EXPECT_TEMPLATE(BOOL, FunctionPointer, __VA_ARGS__)
					EXPECT_FP(TRUE, NormalPtrFun);
					EXPECT_FP(TRUE, PtrFunNoexcept);
					EXPECT_FP(TRUE, VariadicPtrFun);
					EXPECT_FP(TRUE, NormalVariadicPtrFun);
					EXPECT_FP(TRUE, ArgsDiffVariadicPtrFun);

					EXPECT_FP(FALSE, Sig);
					EXPECT_FP(FALSE, ResDiffSig);
					EXPECT_FP(FALSE, ResDiffVariadicSig);
					EXPECT_FP(FALSE, ExpectedVariadicSignature);
					EXPECT_FP(FALSE, NormalRefFun);
					EXPECT_FP(FALSE, RefFunNoexcept);
					EXPECT_FP(FALSE, NormalStdFun);
					EXPECT_FP(FALSE, NormalCallable);
					EXPECT_FP(FALSE, RegularLambda);
					EXPECT_FP(FALSE, NormalMember);
#undef EXPECT_FP
				}

				TEST_F(ParsableConceptTest, TestSTLFunctionLike) {
#define EXPECT_STLFUNLIKE(BOOL, ...) EXPECT_TEMPLATE(BOOL, STLFunctionLike, __VA_ARGS__)
					EXPECT_STLFUNLIKE(TRUE, NormalStdFun);
					EXPECT_STLFUNLIKE(TRUE, ArgsDiffStdFun);
					EXPECT_STLFUNLIKE(TRUE, NormalCallable);
					EXPECT_STLFUNLIKE(TRUE, CallableT2<void(int, float)>);
					EXPECT_STLFUNLIKE(TRUE, CallableT2<void(int, float), DummyStruct>);

					EXPECT_STLFUNLIKE(FALSE, NormalPtrFun);
					EXPECT_STLFUNLIKE(FALSE, PtrFunNoexcept);
					EXPECT_STLFUNLIKE(FALSE, NormalRefFun);
					EXPECT_STLFUNLIKE(FALSE, RefFunNoexcept);
					EXPECT_STLFUNLIKE(FALSE, VariadicPtrFun);
					EXPECT_STLFUNLIKE(FALSE, NormalVariadicPtrFun);
					EXPECT_STLFUNLIKE(FALSE, ArgsDiffVariadicPtrFun);
					EXPECT_STLFUNLIKE(FALSE, Sig);
					EXPECT_STLFUNLIKE(FALSE, ResDiffSig);
					EXPECT_STLFUNLIKE(FALSE, ResDiffVariadicSig);
					EXPECT_STLFUNLIKE(FALSE, ExpectedVariadicSignature);
					EXPECT_STLFUNLIKE(FALSE, RegularLambda);
					EXPECT_STLFUNLIKE(FALSE, NormalMember);
					EXPECT_STLFUNLIKE(FALSE, NonCallableT2<void(int, float)>);
					EXPECT_STLFUNLIKE(FALSE, NonCallableT2<void(int, float), DummyStruct>);
#undef EXPECT_STLFUNLIKE
				}

				TEST_F(ParsableConceptTest, TestMonomorphicFunctor) {
					using type::MonomorphicFunctor;

#define EXPECT_MMF(BOOL, ...) EXPECT_TEMPLATE(BOOL, MonomorphicFunctor, __VA_ARGS__)
					EXPECT_MMF(TRUE, RegularLambda);
					EXPECT_MMF(TRUE, ConstexprLambda);
					EXPECT_MMF(TRUE, MutableLambda);

					EXPECT_MMF(FALSE, Sig);
					EXPECT_MMF(FALSE, ResDiffSig);
					EXPECT_MMF(FALSE, ResDiffVariadicSig);
					EXPECT_MMF(FALSE, ExpectedVariadicSignature);
					EXPECT_MMF(FALSE, NormalPtrFun);
					EXPECT_MMF(FALSE, PtrFunNoexcept);
					EXPECT_MMF(FALSE, NormalRefFun);
					EXPECT_MMF(FALSE, RefFunNoexcept);
					EXPECT_MMF(FALSE, NormalStdFun);
					EXPECT_MMF(FALSE, ArgsDiffStdFun);
					EXPECT_MMF(FALSE, NormalCallable);
					EXPECT_MMF(FALSE, CallableT2<void(int, float)>);
					EXPECT_MMF(FALSE, CallableT2<void(int, float), DummyStruct>);
					EXPECT_MMF(FALSE, VariadicPtrFun);
					EXPECT_MMF(FALSE, NormalVariadicPtrFun);
					EXPECT_MMF(FALSE, ArgsDiffVariadicPtrFun);
					EXPECT_MMF(FALSE, NormalMember);
					EXPECT_MMF(FALSE, NonCallableT2<void(int, float)>);
					EXPECT_MMF(FALSE, NonCallableT2<void(int, float), DummyStruct>);
#undef EXPECT_MMF
				}

				TEST_F(ParsableConceptTest, TestIndirectInvocable) {
#define EXPECT_II(BOOL, ...) EXPECT_TEMPLATE(BOOL, IndirectInvocable, __VA_ARGS__)
					EXPECT_II(TRUE, CallableCallMember);
					EXPECT_II(TRUE, NormalMember);
					EXPECT_II(TRUE, NoexceptMember);
					EXPECT_II(TRUE, ConstMember);
					EXPECT_II(TRUE, ConstRRefMember);
					EXPECT_II(TRUE, ConstLRefNoexceptMember);
					EXPECT_II(TRUE, VirtualMember);
					EXPECT_II(TRUE, OverrideMember);

					EXPECT_II(FALSE, SigNoexcept);
					EXPECT_II(FALSE, NormalPtrFun);
					EXPECT_II(FALSE, PtrFunNoexcept);
					EXPECT_II(FALSE, NormalRefFun);
					EXPECT_II(FALSE, RefFunNoexcept);
					EXPECT_II(FALSE, NormalStdFun);
					EXPECT_II(FALSE, NormalCallable);
					EXPECT_II(FALSE, RegularLambda);
					EXPECT_II(FALSE, ConstexprLambda);
					EXPECT_II(FALSE, MutableLambda);
					EXPECT_II(FALSE, VariadicPtrFun);
#undef EXPECT_II
				}

				TEST_F(ParsableConceptTest, TestMemberFunctionPointer) {
					// `MemberFunctionPointer`
#define EXPECT_MP(BOOL, ...) EXPECT_TEMPLATE(BOOL, MemberFunctionPointer, __VA_ARGS__)
					EXPECT_MP(TRUE, CallableCallMember);
					EXPECT_MP(TRUE, NormalMember);
					EXPECT_MP(TRUE, NoexceptMember);
					EXPECT_MP(TRUE, ConstMember);
					EXPECT_MP(TRUE, ConstRRefMember);
					EXPECT_MP(TRUE, ConstLRefNoexceptMember);
					EXPECT_MP(TRUE, VirtualMember);
					EXPECT_MP(TRUE, OverrideMember);

					EXPECT_MP(FALSE, SigNoexcept);
					EXPECT_MP(FALSE, NormalPtrFun);
					EXPECT_MP(FALSE, PtrFunNoexcept);
					EXPECT_MP(FALSE, NormalRefFun);
					EXPECT_MP(FALSE, RefFunNoexcept);
					EXPECT_MP(FALSE, NormalStdFun);
					EXPECT_MP(FALSE, NormalCallable);
					EXPECT_MP(FALSE, RegularLambda);
					EXPECT_MP(FALSE, ConstexprLambda);
					EXPECT_MP(FALSE, MutableLambda);
					EXPECT_MP(FALSE, VariadicPtrFun);
#undef EXPECT_MP
				}
			}
		}

		TEST_F(HigherLevelUtilitiesTest, TestSameArgsAs) {
			auto pi = std::make_unique<int>(42);
			using ReferencedLambda = decltype([&pi](int, float) -> void {
				*pi = 1;
			});

			EXPECT_TEMPLATE(TRUE, SameArgsAs, ExpectedSignature, ResDiffSig);
			EXPECT_TEMPLATE(TRUE, SameArgsAs, ExpectedSignature, SigNoexcept);
			EXPECT_TEMPLATE(TRUE, SameArgsAs, Sig, NormalPtrFun);
			EXPECT_TEMPLATE(TRUE, SameArgsAs, NormalPtrFun, ResDiffPtrFun);
			EXPECT_TEMPLATE(TRUE, SameArgsAs, NormalPtrFun, PtrFunNoexcept);
			EXPECT_TEMPLATE(TRUE, SameArgsAs, NormalPtrFun, NormalRefFun);
			EXPECT_TEMPLATE(TRUE, SameArgsAs, NormalPtrFun, RefFunNoexcept);
			EXPECT_TEMPLATE(TRUE, SameArgsAs, RegularLambda, ResDiffLambda);
			EXPECT_TEMPLATE(TRUE, SameArgsAs, ResDiffLambda, ReferencedLambda);

			EXPECT_TEMPLATE(FALSE, SameArgsAs, Sig, ArgsDiffStdFun);
			EXPECT_TEMPLATE(FALSE, SameArgsAs, Sig, ArgsDiffCallable);
			EXPECT_TEMPLATE(FALSE, SameArgsAs, Sig, NormalVariadicPtrFun);
		}

		TEST_F(HigherLevelUtilitiesTest, TestSameResultAs) {
			auto pi = std::make_unique<int>(42);
			using ReferencedLambda = decltype([&pi](int, float) -> void {
				*pi = 1;
			});

			EXPECT_TEMPLATE(TRUE, SameResultAs, ExpectedSignature, Sig);
			EXPECT_TEMPLATE(TRUE, SameResultAs, ExpectedSignature, SigNoexcept);
			EXPECT_TEMPLATE(TRUE, SameResultAs, ExpectedSignature, NormalPtrFun);
			EXPECT_TEMPLATE(TRUE, SameResultAs, ExpectedSignature, PtrFunNoexcept);
			EXPECT_TEMPLATE(TRUE, SameResultAs, ExpectedSignature, NormalRefFun);
			EXPECT_TEMPLATE(TRUE, SameResultAs, ExpectedSignature, RefFunNoexcept);
			EXPECT_TEMPLATE(TRUE, SameResultAs, ExpectedSignature, NormalStdFun);
			EXPECT_TEMPLATE(TRUE, SameResultAs, ExpectedSignature, ArgsDiffStdFun);
			EXPECT_TEMPLATE(TRUE, SameResultAs, ExpectedSignature, NormalCallable);
			EXPECT_TEMPLATE(TRUE, SameResultAs, ExpectedSignature, ArgsDiffCallable);
			EXPECT_TEMPLATE(TRUE, SameResultAs, ExpectedSignature, NormalVariadicPtrFun);
			EXPECT_TEMPLATE(TRUE, SameResultAs, ExpectedSignature, RegularLambda);
			EXPECT_TEMPLATE(TRUE, SameResultAs, ExpectedSignature, ReferencedLambda);
			EXPECT_TEMPLATE(TRUE, SameResultAs, ExpectedSignature, NormalVariadicPtrFun);
			EXPECT_TEMPLATE(FALSE, SameResultAs, ExpectedSignature, ResDiffLambda);
			EXPECT_TEMPLATE(FALSE, SameResultAs, ExpectedSignature, ResDiffPtrFun);
			EXPECT_TEMPLATE(FALSE, SameResultAs, ExpectedSignature, ResDiffSig);
		}

		TEST_F(HigherLevelUtilitiesTest, TestFn) {
			auto pi = std::make_unique<int>(42);
			using ReferencedLambda = decltype([&pi](int, float) -> void {
				*pi = 1;
			});

			printType<parse::args_of_t<ExpectedSignature>>("Expected parameter types (Packed): ");
			printType<parse::args_of_t<ExpectedSignature>>("Expected return type: ");
			std::printf("\n");

#define EXPECT_FN(BOOL, FunctionType) EXPECT_TEMPLATE(BOOL, Fn, FunctionType, ExpectedSignature)
			EXPECT_FN(TRUE, SigNoexcept);
			EXPECT_FN(TRUE, NormalPtrFun);
			EXPECT_FN(TRUE, PtrFunNoexcept);
			EXPECT_FN(TRUE, NormalRefFun);
			EXPECT_FN(TRUE, RefFunNoexcept);
			EXPECT_FN(TRUE, NormalStdFun);
			EXPECT_FN(TRUE, NormalCallable);
			EXPECT_FN(TRUE, RegularLambda);
			EXPECT_FN(TRUE, ConstexprLambda);
			EXPECT_FN(TRUE, MutableLambda);
			EXPECT_FN(TRUE, ReferencedLambda);
			// Not `DirectInvocable`
			// Failed requirement check
			EXPECT_FN(FALSE, ResDiffSig);
			EXPECT_FN(FALSE, ResDiffPtrFun);
			EXPECT_FN(FALSE, ArgsDiffStdFun);
			EXPECT_FN(FALSE, ArgsDiffCallable);
			EXPECT_FN(FALSE, ResDiffLambda);
			// Non-function types
			EXPECT_FN(FALSE, int);
			EXPECT_FN(FALSE, decltype(pi));
#undef EXPECT_FN
		}




	}
}

using namespace ::vkz;
using mpl::function::Fn;

template <typename S, Fn<S> F>
void testFn(F f, const int x = 114, const float y = 5.14) {
	f(x, y);
}

TEST(UseTest, TestFn) {
	auto f = [](int x, float y) {
		std::printf("%d, %.2f", x, y);
	};
	using FunctionTypeA = decltype(f);
	using SignatureB = void(int, float);

	static_assert(Fn<FunctionTypeA, SignatureB>);
}
