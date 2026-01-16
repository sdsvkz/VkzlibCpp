#include <functional>
#include <memory>
#include <gtest/gtest.h>

#include "vkzlib/mpl.hpp"

template<typename T>
void printType(const char *name = "T") {
	std::printf("%s = %s\n", name, typeid(T).name());
}

template<typename... Ts>
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

#define POINTER_OF_MEMBER(ClassName, MemberTuple) \
	decltype(&ClassName::VKZLIB_PP_REMOVE_PARENTHESIS MemberTuple)

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

		template<typename R, typename... Args>
		class CallableType<R(Args...)>
		{
		public:
			using FunctionType = R(*)(Args...);

			FunctionType f;

			template<typename F>
			explicit constexpr CallableType(F f) noexcept : f(f) {};

			R call(Args... args) {
				return f(args...);
			}

			R operator() (Args... args) {
				return call(args...);
			}

			using CallOperatorType = POINTER_OF_MEMBER(CallableType, (operator()));
			using CallMemberType = POINTER_OF_MEMBER(CallableType, (call));
		};

		template<typename R, typename... Args>
		class CallableType<R(Args..., ...)>
		{
		public:
			using FunctionType = R(*)(Args..., ...);

			FunctionType f;

			template<typename F>
			explicit constexpr CallableType(F f) noexcept : f(f) {};

			R call(Args... args) {
				return f(args...);
			}

			R operator() (Args... args) {
				return call(args...);
			}

			using CallOperatorType = POINTER_OF_MEMBER(CallableType, (operator()));
			using CallMemberType = POINTER_OF_MEMBER(CallableType, (call));
		};

		template<typename S, typename Dummy = void>
		class CallableT2 {};

		using ExpectedSignature = void(int, float);
		using ExpectedVariadicSignature = void(int, float, ...);

		class SignatureParsingDeps
		{
			using This = SignatureParsingDeps;
		public:
			constexpr SignatureParsingDeps() noexcept = default;

			using Sig = void(int, float);
			using SigNoexcept = void(int, float) noexcept;
			using ResDiffSig = DummyStruct(int, float);
			using ResDiffVariadicSig = DummyStruct(int, float, ...);
		};

		class SignatureParsingTest :
			public SignatureParsingDeps,
			public testing::Test {};

		class DirectInvocableParsingDeps
		{
			using This = DirectInvocableParsingDeps;

		public:
			constexpr DirectInvocableParsingDeps() noexcept = default;

			int iNum { 42 };
			float flNum { 114.514f };
			std::unique_ptr<int> pi { std::make_unique<int>(1919) };

			// Types satisfy `Fn` with expected signature
			using NormalPtrFun = void(*)(int, float);
			using PtrFunNoexcept = void(*)(int, float) noexcept;
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
			char *allDiffN(const int &, DummyStruct) {
				return nullptr;
			}
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

		class ParsingDeps :
			public SignatureParsingDeps,
			public DirectInvocableParsingDeps,
			public MFPParsingDeps {};

		class ParsingTest :
			public ParsingDeps,
			public testing::Test {};

		class ParsableConceptDeps :
			public ParsingDeps {};

		class ParsableConceptTest :
			public ParsableConceptDeps,
			public testing::Test {};

		class PropertyParsingDeps : public ParsingDeps
		{
		public:
			using NormalLambda = decltype([] (int, float) mutable {});
			using NormalOperatorMember = NormalCallable::CallOperatorType;
		};

		class PropertyParsingTest :
			public PropertyParsingDeps,
			public testing::Test {};

		class HigherLevelUtilitiesDeps :
			public ParsingDeps {};

		class HigherLevelUtilitiesTest :
			public HigherLevelUtilitiesDeps,
			public testing::Test {};

		TEST(NonCallableParsingTest, TestParsable) {
			auto pi = std::make_unique<int>(42);

			EXPECT_TEMPLATE(FALSE, Parsable, int);
			EXPECT_TEMPLATE(FALSE, Parsable, decltype(pi));
		}

		TEST_F(SignatureParsingTest, TestParsable) {
			EXPECT_TEMPLATE(TRUE, Parsable, Sig);
			EXPECT_TEMPLATE(TRUE, Parsable, SigNoexcept);
			EXPECT_TEMPLATE(TRUE, Parsable, ResDiffSig);
			EXPECT_TEMPLATE(TRUE, Parsable, ResDiffVariadicSig);
		}

		TEST_F(DirectInvocableParsingTest, TestParsable) {
			auto pi = std::make_unique<int>(42);
			using ReferencedLambda = decltype([&pi](int, float) -> void {
				*pi = 1;
			});

			EXPECT_TEMPLATE(TRUE, Parsable, NormalPtrFun);
			EXPECT_TEMPLATE(TRUE, Parsable, PtrFunNoexcept);
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

		TEST_F(SignatureParsingTest, TestResultOf) {
			EXPECT_TEMPLATE(TRUE, Homogeneous,
				result_of_t<Sig>,
				result_of_t<SigNoexcept>);

			EXPECT_TEMPLATE(FALSE, Homogeneous,
				result_of_t<Sig>,
				result_of_t<ResDiffSig>);

			EXPECT_TEMPLATE(FALSE, Homogeneous,
				result_of_t<Sig>,
				result_of_t<ResDiffVariadicSig>);
		}

		TEST_F(DirectInvocableParsingTest, TestResultOf) {
			auto pi = std::make_unique<int>(42);
			using ReferencedLambda = decltype([&pi](int, float) -> void {
				*pi = 1;
			});

			EXPECT_TEMPLATE(TRUE, Homogeneous,
				result_of_t<NormalPtrFun>,
				result_of_t<PtrFunNoexcept>,
				result_of_t<NormalStdFun>,
				result_of_t<NormalCallable>,
				result_of_t<RegularLambda>,
				result_of_t<ConstexprLambda>,
				result_of_t<MutableLambda>,
				result_of_t<ReferencedLambda>,
				result_of_t<VariadicPtrFun>,
				result_of_t<NormalVariadicPtrFun>,
				result_of_t<ArgsDiffStdFun>,
				result_of_t<ArgsDiffCallable>,
				result_of_t<ArgsDiffVariadicPtrFun>);

			EXPECT_TEMPLATE(FALSE, Homogeneous,
				result_of_t<NormalPtrFun>,
				result_of_t<ResDiffLambda>);

			EXPECT_TEMPLATE(FALSE, Homogeneous,
				result_of_t<NormalPtrFun>,
				result_of_t<ResDiffPtrFun>);

			EXPECT_TEMPLATE(FALSE, Homogeneous,
				result_of_t<NormalPtrFun>,
				result_of_t<AllDiffLambda>);

			EXPECT_TEMPLATE(FALSE, Homogeneous,
				result_of_t<NormalPtrFun>,
				result_of_t<AllDiffVariadicCallable>);
		}

		TEST_F(MFPParsingTest, TestResultOf) {
			EXPECT_TEMPLATE(TRUE, Homogeneous,
				result_of_t<CallableCallMember>,
				result_of_t<NormalMember>,
				result_of_t<NoexceptMember>,
				result_of_t<ConstMember>,
				result_of_t<ConstRRefMember>,
				result_of_t<ConstLRefNoexceptMember>,
				result_of_t<VirtualMember>,
				result_of_t<OverrideMember>,
				result_of_t<ArgsDiffMember>,
				result_of_t<VariadicMember<void, int, int>>,
				result_of_t<NormalVariadicMember<void, int, float>>);

			EXPECT_TEMPLATE(FALSE, Homogeneous,
				result_of_t<NormalMember>,
				result_of_t<ResDiffMember>);

			EXPECT_TEMPLATE(FALSE, Homogeneous,
				result_of_t<NormalMember>,
				result_of_t<AllDiffMember>);

			EXPECT_TEMPLATE(FALSE, Homogeneous,
				result_of_t<NormalMember>,
				result_of_t<VariadicMember<DummyStruct &&, Type::DirectInvocable, Type::IndirectInvocable>>);

			EXPECT_TEMPLATE(FALSE, Homogeneous,
				result_of_t<NormalMember>,
				result_of_t<NormalVariadicMember<std::nullptr_t, int, float>>);
		}

		TEST_F(SignatureParsingTest, TestArgsOf) {
			EXPECT_TEMPLATE(TRUE, Homogeneous,
				args_of_t<SigNoexcept>,
				args_of_t<ResDiffSig>,
				args_of_t<ResDiffVariadicSig>);
		}

		TEST_F(DirectInvocableParsingTest, TestArgsOf) {
			auto pi = std::make_unique<int>(42);
			using ReferencedLambda = decltype([&pi](int, float) -> void {
				*pi = 1;
			});

			EXPECT_TEMPLATE(TRUE, Homogeneous,
				args_of_t<NormalPtrFun>,
				args_of_t<PtrFunNoexcept>,
				args_of_t<NormalStdFun>,
				args_of_t<NormalCallable>,
				args_of_t<RegularLambda>,
				args_of_t<ConstexprLambda>,
				args_of_t<MutableLambda>,
				args_of_t<ReferencedLambda>,
				args_of_t<ResDiffPtrFun>,
				args_of_t<ResDiffLambda>);

			EXPECT_TEMPLATE(FALSE, Homogeneous,
				args_of_t<NormalPtrFun>,
				args_of_t<ArgsDiffStdFun>);

			EXPECT_TEMPLATE(FALSE, Homogeneous,
				args_of_t<NormalPtrFun>,
				args_of_t<ArgsDiffCallable>);

			EXPECT_TEMPLATE(FALSE, Homogeneous,
				args_of_t<NormalPtrFun>,
				args_of_t<ArgsDiffVariadicPtrFun>);
		}

		TEST_F(MFPParsingTest, TestArgsOf) {
			EXPECT_TEMPLATE(TRUE, Homogeneous,
				args_of_t<CallableCallMember>,
				args_of_t<NormalMember>,
				args_of_t<NoexceptMember>,
				args_of_t<ConstMember>,
				args_of_t<ConstRRefMember>,
				args_of_t<ConstLRefNoexceptMember>,
				args_of_t<VirtualMember>,
				args_of_t<OverrideMember>,
				args_of_t<ResDiffMember>,
				args_of_t<VariadicMember<const DummyStruct &, int, float>>,
				args_of_t<NormalVariadicMember<void, int, float>>);

			EXPECT_TEMPLATE(FALSE, Homogeneous,
				args_of_t<NormalMember>,
				args_of_t<ArgsDiffMember>);

			EXPECT_TEMPLATE(FALSE, Homogeneous,
				args_of_t<NormalMember>,
				args_of_t<AllDiffMember>);

			EXPECT_TEMPLATE(FALSE, Homogeneous,
				args_of_t<NormalMember>,
				args_of_t<VariadicMember<const DummyStruct &, int &&, float **>>);

			EXPECT_TEMPLATE(FALSE, Homogeneous,
				args_of_t<NormalMember>,
				args_of_t<NormalVariadicMember<void, float, float>>);
		}

		TEST_F(PropertyParsingTest, TestVariadic) {
			using Property::Concepts::Variadic;

			EXPECT_TEMPLATE(TRUE, Variadic, NormalVariadicPtrFun);
			EXPECT_TEMPLATE(TRUE, Variadic, ResDiffVariadicSig);
			EXPECT_TEMPLATE(TRUE, Variadic, ArgsDiffVariadicPtrFun);
			EXPECT_TEMPLATE(TRUE, Variadic, AllDiffVariadicCallable);
			EXPECT_TEMPLATE(TRUE, Variadic, VariadicMember<void, int, float>);

			EXPECT_TEMPLATE(FALSE, Variadic, NormalPtrFun);
			EXPECT_TEMPLATE(FALSE, Variadic, SigNoexcept);
			EXPECT_TEMPLATE(FALSE, Variadic, AllDiffLambda);
			EXPECT_TEMPLATE(FALSE, Variadic, NormalMember);
			EXPECT_TEMPLATE(FALSE, Variadic, CallableCallMember);

			EXPECT_TEMPLATE(FALSE, Variadic, void);
			EXPECT_TEMPLATE(FALSE, Variadic, int);
		}

		TEST_F(PropertyParsingTest, TestNonCV) {
			using Property::Concepts::NonCV;

#define EXPECT_NONCV(BOOL, ...) EXPECT_TEMPLATE(BOOL, NonCV, __VA_ARGS__)
			EXPECT_NONCV(TRUE, NormalPtrFun);
			EXPECT_NONCV(TRUE, NormalStdFun);
			EXPECT_NONCV(TRUE, NormalCallable);
			EXPECT_NONCV(TRUE, NormalLambda);
			EXPECT_NONCV(TRUE, NormalVariadicPtrFun);
			EXPECT_NONCV(TRUE, NormalMember);
			EXPECT_NONCV(TRUE, NormalOperatorMember);
			EXPECT_NONCV(TRUE, SigNoexcept);
			EXPECT_NONCV(TRUE, PtrFunNoexcept);
			EXPECT_NONCV(TRUE, NoexceptMember);

			EXPECT_NONCV(FALSE, ConstLRefNoexceptMember);
			EXPECT_NONCV(FALSE, VariadicMember<void, int, float>);

			EXPECT_NONCV(FALSE, void);
			EXPECT_NONCV(FALSE, int);
#undef EXPECT_NONCV
		}

		TEST_F(PropertyParsingTest, TestConst) {
			using Property::Concepts::Const;

#define EXPECT_CONST(BOOL, ...) EXPECT_TEMPLATE(BOOL, Const, __VA_ARGS__)
			EXPECT_CONST(TRUE, RegularLambda);
			EXPECT_CONST(TRUE, ConstexprLambda);
			EXPECT_CONST(TRUE, ConstMember);
			EXPECT_CONST(TRUE, ConstLRefNoexceptMember);
			EXPECT_CONST(TRUE, ConstRRefMember);

			EXPECT_CONST(FALSE, NormalPtrFun);
			EXPECT_CONST(FALSE, NormalStdFun);
			EXPECT_CONST(FALSE, NormalCallable);
			EXPECT_CONST(FALSE, NormalLambda);
			EXPECT_CONST(FALSE, NormalVariadicPtrFun);
			EXPECT_CONST(FALSE, NormalMember);
			EXPECT_CONST(FALSE, NormalOperatorMember);
			EXPECT_CONST(FALSE, Sig);
			EXPECT_CONST(FALSE, SigNoexcept);
			EXPECT_CONST(FALSE, PtrFunNoexcept);
			EXPECT_CONST(FALSE, MutableLambda);

			EXPECT_CONST(FALSE, void);
			EXPECT_CONST(FALSE, int);
#undef EXPECT_CONST
		}

		TEST_F(PropertyParsingTest, TestVolatile) {
			using Property::Concepts::Volatile;

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
			EXPECT_VOLATILE(FALSE, NormalStdFun);
			EXPECT_VOLATILE(FALSE, NormalCallable);
			EXPECT_VOLATILE(FALSE, NormalLambda);
			EXPECT_VOLATILE(FALSE, NormalVariadicPtrFun);
			EXPECT_VOLATILE(FALSE, NormalMember);
			EXPECT_VOLATILE(FALSE, NormalOperatorMember);
			EXPECT_VOLATILE(FALSE, Sig);
			EXPECT_VOLATILE(FALSE, SigNoexcept);
			EXPECT_VOLATILE(FALSE, PtrFunNoexcept);
			EXPECT_VOLATILE(FALSE, MutableLambda);

			EXPECT_VOLATILE(FALSE, void);
			EXPECT_VOLATILE(FALSE, int);
#undef EXPECT_VOLATILE
		}

		template<typename T>
		concept ConstVolatile = Property::Concepts::Const<T> && Property::Concepts::Volatile<T>;

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
			EXPECT_CONST_VOLATILE(FALSE, NormalStdFun);
			EXPECT_CONST_VOLATILE(FALSE, NormalCallable);
			EXPECT_CONST_VOLATILE(FALSE, NormalLambda);
			EXPECT_CONST_VOLATILE(FALSE, NormalVariadicPtrFun);
			EXPECT_CONST_VOLATILE(FALSE, NormalMember);
			EXPECT_CONST_VOLATILE(FALSE, NormalOperatorMember);
			EXPECT_CONST_VOLATILE(FALSE, Sig);
			EXPECT_CONST_VOLATILE(FALSE, SigNoexcept);
			EXPECT_CONST_VOLATILE(FALSE, PtrFunNoexcept);
			EXPECT_CONST_VOLATILE(FALSE, MutableLambda);

			EXPECT_CONST_VOLATILE(FALSE, void);
			EXPECT_CONST_VOLATILE(FALSE, int);
#undef EXPECT_CONST_VOLATILE
		}

		TEST_F(PropertyParsingTest, TestNonRef) {
			using Property::Concepts::NonRef;

#define EXPECT_NONREF(BOOL, ...) EXPECT_TEMPLATE(BOOL, NonRef, __VA_ARGS__)
			EXPECT_NONREF(TRUE, NormalPtrFun);
			EXPECT_NONREF(TRUE, NormalStdFun);
			EXPECT_NONREF(TRUE, NormalCallable);
			EXPECT_NONREF(TRUE, NormalLambda);
			EXPECT_NONREF(TRUE, NormalVariadicPtrFun);
			EXPECT_NONREF(TRUE, NormalMember);
			EXPECT_NONREF(TRUE, NormalOperatorMember);

			EXPECT_NONREF(TRUE, SigNoexcept);
			EXPECT_NONREF(TRUE, PtrFunNoexcept);
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
			using Property::Concepts::LValueRef;

#define  EXPECT_LVALUEREF(BOOL, ...) EXPECT_TEMPLATE(BOOL, LValueRef, __VA_ARGS__)
			EXPECT_LVALUEREF(TRUE, ConstLRefNoexceptMember);
			EXPECT_LVALUEREF(TRUE, VariadicMember<void, int, float>);

			EXPECT_LVALUEREF(FALSE, NormalPtrFun);
			EXPECT_LVALUEREF(FALSE, NormalStdFun);
			EXPECT_LVALUEREF(FALSE, NormalCallable);
			EXPECT_LVALUEREF(FALSE, NormalLambda);
			EXPECT_LVALUEREF(FALSE, NormalVariadicPtrFun);
			EXPECT_LVALUEREF(FALSE, NormalMember);
			EXPECT_LVALUEREF(FALSE, NormalOperatorMember);

			EXPECT_LVALUEREF(FALSE, SigNoexcept);
			EXPECT_LVALUEREF(FALSE, PtrFunNoexcept);
			EXPECT_LVALUEREF(FALSE, NoexceptMember);

			EXPECT_LVALUEREF(FALSE, ConstRRefMember);
			EXPECT_LVALUEREF(FALSE, ResDiffMember);
			EXPECT_LVALUEREF(FALSE, BuffedMember);

			EXPECT_LVALUEREF(FALSE, int);
			EXPECT_LVALUEREF(FALSE, float);
#undef EXPECT_LVALUEREF
		}

		TEST_F(PropertyParsingTest, TestRValueRef) {
			using Property::Concepts::RValueRef;

#define EXPECT_RVALUEREF(BOOL, ...) EXPECT_TEMPLATE(BOOL, RValueRef, __VA_ARGS__)
			EXPECT_RVALUEREF(TRUE, ConstRRefMember);
			EXPECT_RVALUEREF(TRUE, ResDiffMember);
			EXPECT_RVALUEREF(TRUE, BuffedMember);

			EXPECT_RVALUEREF(FALSE, ConstLRefNoexceptMember);
			EXPECT_RVALUEREF(FALSE, VariadicMember<void, int, float>);

			EXPECT_RVALUEREF(FALSE, NormalPtrFun);
			EXPECT_RVALUEREF(FALSE, NormalStdFun);
			EXPECT_RVALUEREF(FALSE, NormalCallable);
			EXPECT_RVALUEREF(FALSE, NormalLambda);
			EXPECT_RVALUEREF(FALSE, NormalVariadicPtrFun);
			EXPECT_RVALUEREF(FALSE, NormalMember);
			EXPECT_RVALUEREF(FALSE, NormalOperatorMember);

			EXPECT_RVALUEREF(FALSE, SigNoexcept);
			EXPECT_RVALUEREF(FALSE, PtrFunNoexcept);
			EXPECT_RVALUEREF(FALSE, NoexceptMember);

			EXPECT_RVALUEREF(FALSE, int);
			EXPECT_RVALUEREF(FALSE, float);
#undef EXPECT_RVALUEREF
		}

		TEST_F(PropertyParsingTest, TestNoThrow) {
			using Property::Concepts::NoThrow;

#define EXPECT_NOTHROW(BOOL, ...) EXPECT_TEMPLATE(BOOL, NoThrow, __VA_ARGS__)
			EXPECT_NOTHROW(TRUE, SigNoexcept);
			EXPECT_NOTHROW(TRUE, PtrFunNoexcept);
			EXPECT_NOTHROW(TRUE, NoexceptMember);
			EXPECT_NOTHROW(TRUE, ResDiffMember);
			EXPECT_NOTHROW(TRUE, ConstLRefNoexceptMember);
			EXPECT_NOTHROW(TRUE, BuffedMember);
			EXPECT_NOTHROW(TRUE, VariadicMember<void, int, float>);


			EXPECT_NOTHROW(FALSE, NormalPtrFun);
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
			using Property::Concepts::Normal;

#define EXPECT_NORMAL(BOOL, ...) EXPECT_TEMPLATE(BOOL, Normal, __VA_ARGS__)
			EXPECT_NORMAL(TRUE, NormalPtrFun);
			EXPECT_NORMAL(TRUE, NormalStdFun);
			EXPECT_NORMAL(TRUE, NormalCallable);
			EXPECT_NORMAL(TRUE, NormalLambda);
			EXPECT_NORMAL(TRUE, NormalVariadicPtrFun);
			EXPECT_NORMAL(TRUE, NormalMember);
			EXPECT_NORMAL(TRUE, NormalOperatorMember);

			EXPECT_NORMAL(FALSE, SigNoexcept);
			EXPECT_NORMAL(FALSE, PtrFunNoexcept);
			EXPECT_NORMAL(FALSE, NoexceptMember);
			EXPECT_NORMAL(FALSE, ConstLRefNoexceptMember);
			EXPECT_NORMAL(FALSE, VariadicMember<void, int, float>);

			EXPECT_NORMAL(FALSE, void);
			EXPECT_NORMAL(FALSE, int);
#undef EXPECT_NORMAL
		}

		TEST_F(ParsableConceptTest, TestSignatureContainer) {
			using Type::Concepts::SignatureContainer;

#define EXPECT_SC(BOOL, ...) EXPECT_TEMPLATE(BOOL, SignatureContainer, __VA_ARGS__)
			EXPECT_SC(TRUE, ExpectedSignature);
			EXPECT_SC(TRUE, SigNoexcept);
			EXPECT_SC(TRUE, ResDiffSig);
			EXPECT_SC(TRUE, ResDiffVariadicSig);
			EXPECT_SC(TRUE, ExpectedVariadicSignature);

			EXPECT_SC(FALSE, PtrFunNoexcept);
			EXPECT_SC(FALSE, NormalStdFun);
			EXPECT_SC(FALSE, NormalCallable);
			EXPECT_SC(FALSE, RegularLambda);
			EXPECT_SC(FALSE, NormalMember);
			EXPECT_SC(FALSE, NormalVariadicPtrFun);
#undef EXPECT_SC
		}

		TEST_F(ParsableConceptTest, TestSignature) {
			using Type::Concepts::Signature;

#define EXPECT_SIG(BOOL, ...) EXPECT_TEMPLATE(BOOL, Signature, __VA_ARGS__)
			EXPECT_SIG(TRUE, ExpectedSignature);
			EXPECT_SIG(TRUE, SigNoexcept);
			EXPECT_SIG(TRUE, ResDiffSig);
			EXPECT_SIG(TRUE, ResDiffVariadicSig);
			EXPECT_SIG(TRUE, ExpectedVariadicSignature);

			EXPECT_SIG(FALSE, PtrFunNoexcept);
			EXPECT_SIG(FALSE, NormalStdFun);
			EXPECT_SIG(FALSE, NormalCallable);
			EXPECT_SIG(FALSE, RegularLambda);
			EXPECT_SIG(FALSE, NormalMember);
			EXPECT_SIG(FALSE, NormalVariadicPtrFun);
		}

		TEST_F(ParsableConceptTest, TestDirectInvocable) {
			using Type::Concepts::DirectInvocable;

#define EXPECT_DI(BOOL, ...) EXPECT_TEMPLATE(BOOL, DirectInvocable, __VA_ARGS__)
			EXPECT_DI(TRUE, NormalPtrFun);
			EXPECT_DI(TRUE, PtrFunNoexcept);
			EXPECT_DI(TRUE, NormalStdFun);
			EXPECT_DI(TRUE, NormalCallable);
			EXPECT_DI(TRUE, RegularLambda);
			EXPECT_DI(TRUE, ConstexprLambda);
			EXPECT_DI(TRUE, MutableLambda);
			EXPECT_DI(TRUE, VariadicPtrFun);

			EXPECT_DI(FALSE, SigNoexcept);
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

		TEST_F(ParsableConceptTest, TestIndirectInvocable) {
			using Type::Concepts::IndirectInvocable;

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
			using Type::Concepts::MemberFunctionPointer;
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
			EXPECT_MP(FALSE, NormalStdFun);
			EXPECT_MP(FALSE, NormalCallable);
			EXPECT_MP(FALSE, RegularLambda);
			EXPECT_MP(FALSE, ConstexprLambda);
			EXPECT_MP(FALSE, MutableLambda);
			EXPECT_MP(FALSE, VariadicPtrFun);
#undef EXPECT_MP
		}

		TEST_F(HigherLevelUtilitiesTest, TestSameArgsAs) {
			auto pi = std::make_unique<int>(42);
			using ReferencedLambda = decltype([&pi](int, float) -> void {
				*pi = 1;
			});

			EXPECT_TEMPLATE(TRUE, SameArgsAs, ExpectedSignature, ResDiffSig);
			EXPECT_TEMPLATE(TRUE, SameArgsAs, ExpectedSignature, SigNoexcept);
			EXPECT_TEMPLATE(TRUE, SameArgsAs, NormalPtrFun, ResDiffPtrFun);
			EXPECT_TEMPLATE(TRUE, SameArgsAs, NormalPtrFun, PtrFunNoexcept);
			EXPECT_TEMPLATE(TRUE, SameArgsAs, RegularLambda, ResDiffLambda);
			EXPECT_TEMPLATE(TRUE, SameArgsAs, ResDiffLambda, ReferencedLambda);
			EXPECT_TEMPLATE(FALSE, SameArgsAs, NormalStdFun, ArgsDiffStdFun);
			EXPECT_TEMPLATE(FALSE, SameArgsAs, NormalCallable, ArgsDiffCallable);
			EXPECT_TEMPLATE(FALSE, SameArgsAs, NormalPtrFun, NormalVariadicPtrFun);
		}

		TEST_F(HigherLevelUtilitiesTest, TestSameResultAs) {
			auto pi = std::make_unique<int>(42);
			using ReferencedLambda = decltype([&pi](int, float) -> void {
				*pi = 1;
			});

			EXPECT_TEMPLATE(TRUE, SameResultAs, ExpectedSignature, SigNoexcept);
			EXPECT_TEMPLATE(TRUE, SameResultAs, NormalPtrFun, PtrFunNoexcept);
			EXPECT_TEMPLATE(TRUE, SameResultAs, NormalStdFun, ArgsDiffStdFun);
			EXPECT_TEMPLATE(TRUE, SameResultAs, NormalCallable, ArgsDiffCallable);
			EXPECT_TEMPLATE(TRUE, SameResultAs, NormalPtrFun, NormalVariadicPtrFun);
			EXPECT_TEMPLATE(TRUE, SameResultAs, RegularLambda, ReferencedLambda);
			EXPECT_TEMPLATE(FALSE, SameResultAs, RegularLambda, ResDiffLambda);
			EXPECT_TEMPLATE(FALSE, SameResultAs, NormalPtrFun, ResDiffPtrFun);
			EXPECT_TEMPLATE(FALSE, SameResultAs, ExpectedSignature, ResDiffSig);
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
			EXPECT_FN(TRUE, NormalPtrFun);
			EXPECT_FN(TRUE, PtrFunNoexcept);
			EXPECT_FN(TRUE, NormalStdFun);
			EXPECT_FN(TRUE, NormalCallable);
			EXPECT_FN(TRUE, RegularLambda);
			EXPECT_FN(TRUE, ConstexprLambda);
			EXPECT_FN(TRUE, MutableLambda);
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
