#ifndef VKZLIB_TEST_MPL_FUNCTION_TEST_DEPS_HPP
#define VKZLIB_TEST_MPL_FUNCTION_TEST_DEPS_HPP

#include <functional>
#include <memory>

#include <gtest/gtest.h>

#include <vkzlib/mpl/common.hpp>

#include <common.hpp>

namespace MplFunctionTest {
	using ::vkz::mpl::Homogeneous;
	using namespace ::vkz::mpl;
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
}

#endif // VKZLIB_TEST_MPL_FUNCTION_TEST_DEPS_HPP