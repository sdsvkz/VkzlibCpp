#include <functional>
#include <cstdio>
#include <memory>
#include <gtest/gtest.h>

#include "vkzlib/mpl.hpp"

template<typename T>
void printType(const char *name = "T") {
	std::printf("%s = %s\n", name, typeid(T).name());
}

template<typename ...Ts>
void printAllTypes() {
	int i = 0;
	(std::printf("%d: %s\n", ++i, typeid(Ts).name()), ...);
}

// `TRUE` or `FALSE`
#define EXPECT_TEMPLATE(BOOL, Concept, ...)							\
	do {															\
		constexpr bool e = Concept<__VA_ARGS__>;					\
		std::printf("\n" #Concept " with template parameters: \n");	\
		printAllTypes<__VA_ARGS__>();								\
		std::printf("Evaluted to %s\n", e ? "true" : "false");		\
		EXPECT_##BOOL(e);											\
	} while (false);

#define POINTER_OF_MEMBER(ClassName, MemberName) \
	decltype(&ClassName::MemberName)

namespace Test::vkz::mpl {
	using namespace ::vkz::mpl;

	namespace core {

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

	}

	namespace function {

		using namespace ::vkz::mpl::function;

		struct DummyStruct {};

		template<typename S>
		class CallableType;

		template<typename R, typename ...Args>
		class CallableType<R(Args...)>
		{
		public:
			using FunctionType = R(*)(Args...);

			FunctionType f;

			template<typename F>
			CallableType(F f) : f(f) {};

			R call(Args ...args) {
				return f(args...);
			}

			R variadic(Args ...args, ...) const &noexcept {
				return f(args...);
			}

			R normalVariadic(Args ...args, ...) {
				return f(args...);
			}

			R operator() (Args ...args) {
				return call(args...);
			}

			using CallOperatorType = POINTER_OF_MEMBER(CallableType, operator());
			using CallMemberType = POINTER_OF_MEMBER(CallableType, call);
			using VariadicMemberType = POINTER_OF_MEMBER(CallableType, variadic);
			using NormalVariadicMemberType = POINTER_OF_MEMBER(CallableType, normalVariadic);
		};

		using ExpectedSignature = void(int, float);
		using ExpectedVariadicSignature = void(int, float, ...);

		class BaseFunctionParsingTest : public testing::Test
		{
			using This = BaseFunctionParsingTest;

		public:
			BaseFunctionParsingTest() noexcept = default;

			void f(int, float) {}
			void g(int, float) noexcept {}
			void h(int, float) const {}
			void i(int, float) const noexcept {}
			void j(int, float) const &noexcept {}
			virtual void k(int, float) const noexcept {}

			int iNum { 42 };
			float flNum { 114.514f };
			std::unique_ptr<int> pi { std::make_unique<int>(1919) };

			// Types satisfy `Fn` with expected signature
			using SigNoexcept = void(int, float) noexcept;
			using PtrFun = void(*)(int, float);
			using PtrFunNoexcept = void(*)(int, float) noexcept;
			using StdFun = std::function<void(int, float)>;
			using Callable = CallableType<void(int, float)>;
			using Lambda = decltype([pi = std::make_unique<int>(42)](int, float) -> void {
				*pi = 1;
			});
			using ConstexprLambda = decltype([](int, float) constexpr noexcept -> void {});
			using QualifiedLambda = decltype([ptr = std::make_unique<int>(42)](int, float) mutable -> void {
				*ptr = 1;
			});
			// Types not satisfy `Fn` with expected signature
			using ResDiffSig = DummyStruct(int, float);
			using ResDiffPtrFun = DummyStruct(*)(int, float);
			using ArgsDiffStdFun = std::function<void(int, const float &)>;
			using ArgsDiffCallable = CallableType<void(int, int)>;
			using ResDiffLambda = decltype([pi = std::make_unique<int>(42)](int, float) {
				return *pi;
			});
			// Types satisfy `Fn` with expected variadic signature
			using VariadicPtrFun = void(*)(int, float, ...);
			// Types not satisfy `Fn` with expected variadic signature
			using InvalidVariadicPtrFun = void(*)(int, const char *, ...);
			// Member function pointers satisfy `Fn` with expected signature
			using MemberF = POINTER_OF_MEMBER(This, f);
			using MemberG = POINTER_OF_MEMBER(This, g);
			using MemberH = POINTER_OF_MEMBER(This, h);
			using MemberI = POINTER_OF_MEMBER(This, i);
			using MemberJ = POINTER_OF_MEMBER(This, j);
			using MemberK = POINTER_OF_MEMBER(This, k);
		};

		class OverrideMemberParsingTest : public BaseFunctionParsingTest {
			using This = OverrideMemberParsingTest;

		public:
			OverrideMemberParsingTest() noexcept = default;

			void k(int, float) const noexcept override {}

			using MemberOverloadedJ = POINTER_OF_MEMBER(This, j);
		};

		class HigherLevelUtilitiesTest : public BaseFunctionParsingTest {
			using This = HigherLevelUtilitiesTest;

		public:

		};

		TEST_F(BaseFunctionParsingTest, TestParsable) {
			auto pi = std::make_unique<int>(42);
			using ReferencedLambda = decltype([&pi](int, float) -> void {
				*pi = 1;
			});

			EXPECT_TEMPLATE(TRUE, Parsable, SigNoexcept);
			EXPECT_TEMPLATE(TRUE, Parsable, PtrFun);
			EXPECT_TEMPLATE(TRUE, Parsable, PtrFunNoexcept);
			EXPECT_TEMPLATE(TRUE, Parsable, StdFun);
			EXPECT_TEMPLATE(TRUE, Parsable, Callable);
			EXPECT_TEMPLATE(TRUE, Parsable, Lambda);
			EXPECT_TEMPLATE(TRUE, Parsable, ConstexprLambda);
			EXPECT_TEMPLATE(TRUE, Parsable, QualifiedLambda);
			EXPECT_TEMPLATE(TRUE, Parsable, ReferencedLambda);
			EXPECT_TEMPLATE(TRUE, Parsable, ResDiffSig);
			EXPECT_TEMPLATE(TRUE, Parsable, ResDiffPtrFun);
			EXPECT_TEMPLATE(TRUE, Parsable, ArgsDiffStdFun);
			EXPECT_TEMPLATE(TRUE, Parsable, ArgsDiffCallable);
			EXPECT_TEMPLATE(TRUE, Parsable, ResDiffLambda);
			EXPECT_TEMPLATE(FALSE, Parsable, int);
			EXPECT_TEMPLATE(FALSE, Parsable, decltype(pi));
		}

		TEST_F(BaseFunctionParsingTest, TestWithVariadicParam) {
			EXPECT_TEMPLATE(TRUE, WithVariadicParam, VariadicPtrFun);
			EXPECT_TEMPLATE(TRUE, WithVariadicParam, InvalidVariadicPtrFun);
			EXPECT_TEMPLATE(FALSE, WithVariadicParam, PtrFun);
			EXPECT_TEMPLATE(FALSE, WithVariadicParam, SigNoexcept);
		}

		template<typename T>
		concept NormalMFP =
			ParsableType::MemberFunctionPointer<T> &&
			Normal<T>;

		template <typename T>
		concept NormalVariadicMFP =
			NormalMFP<T> &&
			WithVariadicParam<T>;

		TEST_F(OverrideMemberParsingTest, TestMemberFunctionPointerParsing) {

			// `MemberFunctionPointer`
#define EXPECT_MP(BOOL, TypeName) EXPECT_TEMPLATE(BOOL, ParsableType::MemberFunctionPointer, TypeName)
			EXPECT_MP(TRUE, MemberF);
			EXPECT_MP(TRUE, MemberG);
			EXPECT_MP(TRUE, MemberH);
			EXPECT_MP(TRUE, MemberI);
			EXPECT_MP(TRUE, MemberJ);
			EXPECT_MP(TRUE, MemberOverloadedJ);
			EXPECT_MP(TRUE, MemberK);
			EXPECT_MP(TRUE, Callable::NormalVariadicMemberType);
			EXPECT_MP(TRUE, Callable::VariadicMemberType);
			[[maybe_unused]] constexpr bool _e = ParsableType::MemberFunctionPointer<SigNoexcept>;
			EXPECT_MP(FALSE, SigNoexcept);
			EXPECT_MP(FALSE, PtrFun);
			EXPECT_MP(FALSE, PtrFunNoexcept);
			EXPECT_MP(FALSE, StdFun);
			EXPECT_MP(FALSE, Callable);
			EXPECT_MP(FALSE, Lambda);
			EXPECT_MP(FALSE, ConstexprLambda);
			EXPECT_MP(FALSE, QualifiedLambda);
			EXPECT_MP(FALSE, VariadicPtrFun);
#undef EXPECT_MP

			// `MemberFunctionPointer && Normal`
#define EXPECT_NORMAL_MFP(BOOL, TypeName) EXPECT_TEMPLATE(BOOL,	NormalMFP, TypeName)
			EXPECT_NORMAL_MFP(TRUE, MemberF);
			EXPECT_NORMAL_MFP(TRUE, Callable::CallMemberType);
			EXPECT_NORMAL_MFP(TRUE, Callable::CallOperatorType);
			EXPECT_NORMAL_MFP(TRUE, Callable::NormalVariadicMemberType);
			EXPECT_NORMAL_MFP(FALSE, MemberJ);
			EXPECT_NORMAL_MFP(FALSE, Callable::VariadicMemberType);
#undef EXPECT_NORMAL_MFP

			// `result_of_t` for `MemberFunctionPointer`
			using ResF = result_of_t<MemberF>;
			using ResG = result_of_t<MemberG>;
			using ResH = result_of_t<MemberH>;
			using ResI = result_of_t<MemberI>;
			using ResJ = result_of_t<MemberJ>;
			using ResOverloadedJ = result_of_t<MemberOverloadedJ>;
			using ResK = result_of_t<MemberK>;
			using ResNVar = result_of_t<Callable::NormalVariadicMemberType>;
			using ResVar = result_of_t<Callable::VariadicMemberType>;

			EXPECT_TEMPLATE(TRUE, Homogeneous, ResF, ResG, ResH, ResI, ResJ, ResOverloadedJ, ResK);
			EXPECT_TEMPLATE(TRUE, Homogeneous, ResNVar, ResVar);

			// `args_of_t` for `MemberFunctionPointer`
			using ArgsF = args_of_t<MemberF>;
			using ArgsG = args_of_t<MemberG>;
			using ArgsH = args_of_t<MemberH>;
			using ArgsI = args_of_t<MemberI>;
			using ArgsJ = args_of_t<MemberJ>;
			using ArgsOverloadedJ = args_of_t<MemberOverloadedJ>;
			using ArgsK = args_of_t<MemberK>;
			using ArgsNVar = args_of_t<Callable::NormalVariadicMemberType>;
			using ArgsVar = args_of_t<Callable::VariadicMemberType>;

			EXPECT_TEMPLATE(TRUE, Homogeneous, ArgsF, ArgsG, ArgsH, ArgsI, ArgsJ, ArgsOverloadedJ, ArgsK);
			EXPECT_TEMPLATE(TRUE, Homogeneous, ArgsNVar, ArgsVar);
			EXPECT_TEMPLATE(TRUE, NormalVariadicMFP, Callable::NormalVariadicMemberType);
			EXPECT_TEMPLATE(FALSE, NormalVariadicMFP, Callable::VariadicMemberType);
		}

		TEST_F(HigherLevelUtilitiesTest, TestSameArgsAs) {
			auto pi = std::make_unique<int>(42);
			using ReferencedLambda = decltype([&pi](int, float) -> void {
				*pi = 1;
			});

			EXPECT_TEMPLATE(TRUE, Homogeneous,
				args_of_t<SigNoexcept>,
				args_of_t<PtrFun>,
				args_of_t<PtrFunNoexcept>,
				args_of_t<StdFun>,
				args_of_t<Callable>,
				args_of_t<Lambda>,
				args_of_t<ConstexprLambda>,
				args_of_t<QualifiedLambda>,
				args_of_t<ReferencedLambda>);

			EXPECT_TEMPLATE(TRUE, SameArgsAs, ExpectedSignature, ResDiffSig);
			EXPECT_TEMPLATE(TRUE, SameArgsAs, ExpectedSignature, SigNoexcept);
			EXPECT_TEMPLATE(TRUE, SameArgsAs, PtrFun, ResDiffPtrFun);
			EXPECT_TEMPLATE(TRUE, SameArgsAs, PtrFun, PtrFunNoexcept);
			EXPECT_TEMPLATE(FALSE, SameArgsAs, StdFun, ArgsDiffStdFun);
			EXPECT_TEMPLATE(FALSE, SameArgsAs, Callable, ArgsDiffCallable);
			EXPECT_TEMPLATE(TRUE, SameArgsAs, Lambda, ResDiffLambda);
			EXPECT_TEMPLATE(FALSE, SameArgsAs, PtrFun, VariadicPtrFun);
		}

		TEST_F(HigherLevelUtilitiesTest, TestSameResultAs) {
			auto pi = std::make_unique<int>(42);
			using ReferencedLambda = decltype([&pi](int, float) -> void {
				*pi = 1;
			});

			EXPECT_TEMPLATE(TRUE, Homogeneous,
				result_of_t<SigNoexcept>,
				result_of_t<PtrFun>,
				result_of_t<PtrFunNoexcept>,
				result_of_t<StdFun>,
				result_of_t<Callable>,
				result_of_t<Lambda>,
				result_of_t<ConstexprLambda>,
				result_of_t<QualifiedLambda>,
				result_of_t<ReferencedLambda>
			);

			EXPECT_TEMPLATE(FALSE, SameResultAs, ExpectedSignature, ResDiffSig);
			EXPECT_TEMPLATE(TRUE, SameResultAs, ExpectedSignature, SigNoexcept);
			EXPECT_TEMPLATE(FALSE, SameResultAs, PtrFun, ResDiffPtrFun);
			EXPECT_TEMPLATE(TRUE, SameResultAs, PtrFun, PtrFunNoexcept);
			EXPECT_TEMPLATE(TRUE, SameResultAs, StdFun, ArgsDiffStdFun);
			EXPECT_TEMPLATE(TRUE, SameResultAs, Callable, ArgsDiffCallable);
			EXPECT_TEMPLATE(FALSE, SameResultAs, Lambda, ResDiffLambda);
			EXPECT_TEMPLATE(TRUE, SameResultAs, PtrFun, VariadicPtrFun);
		}

		TEST_F(HigherLevelUtilitiesTest, TestFn) {
			auto pi = std::make_unique<int>(42);
			using ReferencedLambda = decltype([&pi](int, float) -> void {
				*pi = 1;
			});

			printType<args_of_t<ExpectedSignature>>("Expected parameter types (Packed): ");
			printType<args_of_t<ExpectedSignature>>("Expected return type: ");
			std::printf("\n");

#define EXPECT_FN(BOOL, FunctionType) EXPECT_TEMPLATE(BOOL, Fn, FunctionType, ExpectedSignature)
			EXPECT_FN(TRUE, PtrFun);
			EXPECT_FN(TRUE, PtrFunNoexcept);
			EXPECT_FN(TRUE, StdFun);
			EXPECT_FN(TRUE, Callable);
			EXPECT_FN(TRUE, Lambda);
			EXPECT_FN(TRUE, ConstexprLambda);
			EXPECT_FN(TRUE, QualifiedLambda);
			EXPECT_FN(TRUE, ReferencedLambda);
			// Not `DirectInvocable`
			EXPECT_FN(FALSE, SigNoexcept);
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
