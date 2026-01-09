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

#define EXPECT_TEMPLATE_TRUE(Concept, ...)							\
	do {															\
		constexpr bool e = Concept<__VA_ARGS__>;					\
		std::printf("\n" #Concept " with template parameters: \n");	\
		printAllTypes<__VA_ARGS__>();								\
		std::printf("Evaluted to %s\n", e ? "true" : "false");		\
		EXPECT_TRUE(e);												\
	} while (false);

#define EXPECT_TEMPLATE_FALSE(Concept, ...)							\
	do {															\
		constexpr bool e = Concept<__VA_ARGS__>;					\
		std::printf("\n" #Concept " with template parameters: \n");	\
		printAllTypes<__VA_ARGS__>();								\
		std::printf("Evaluted to %s\n", e ? "true" : "false");		\
		EXPECT_FALSE(e);											\
	} while (false);

#define POINTER_OF_MEMBER(ClassName, MemberName) \
	decltype(&ClassName::MemberName)

namespace Test {
	namespace vkz {
		namespace mpl {
			namespace core {

				TEST(MplCoreTest, TestHomogeneous) {
					using A = int;
					using B = A;
					using C = B;
					using D = int;
					using E = const int;
					using F = const int &;
					EXPECT_TEMPLATE_TRUE(::vkz::mpl::Homogeneous, A, B, C, D);
					EXPECT_TEMPLATE_FALSE(::vkz::mpl::Homogeneous, D, E, F);
					EXPECT_TEMPLATE_FALSE(::vkz::mpl::Homogeneous, E, B, C);
					EXPECT_TEMPLATE_FALSE(::vkz::mpl:: Homogeneous, B, E, C);
					EXPECT_TEMPLATE_FALSE(::vkz::mpl:: Homogeneous, B, C, E);
				}

			}

			namespace function {

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

				class FnTest : public testing::Test
				{
				public:
					FnTest() noexcept {}

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
					using MemberF = POINTER_OF_MEMBER(FnTest, f);
					using MemberG = POINTER_OF_MEMBER(FnTest, g);
					using MemberH = POINTER_OF_MEMBER(FnTest, h);
					using MemberI = POINTER_OF_MEMBER(FnTest, i);
					using MemberJ = POINTER_OF_MEMBER(FnTest, j);
					using MemberK = POINTER_OF_MEMBER(FnTest, k);
				};

				class MemberSignatureParsingTest : public FnTest {
				public:
					MemberSignatureParsingTest() noexcept {}

					void k(int, float) const noexcept override {}

					using MemberOverloadedJ = POINTER_OF_MEMBER(MemberSignatureParsingTest, j);
				};

				TEST_F(MemberSignatureParsingTest, TestNormalMemberSignatureParsing) {
#define EXPECT_NORMAL_MEMBER_SIGNATURE(TypeName) \
	EXPECT_TEMPLATE_TRUE(::vkz::mpl::function::NormalMemberSignature, TypeName)
					EXPECT_NORMAL_MEMBER_SIGNATURE(MemberF);
					EXPECT_NORMAL_MEMBER_SIGNATURE(Callable::CallMemberType);
					EXPECT_NORMAL_MEMBER_SIGNATURE(Callable::CallOperatorType);
					EXPECT_NORMAL_MEMBER_SIGNATURE(Callable::NormalVariadicMemberType);
#undef EXPECT_NORMAL_MEMBER_SIGNATURE
#define EXPECT_NOT_NORMAL_MEMBER_SIGNATURE(TypeName) \
	EXPECT_TEMPLATE_FALSE(::vkz::mpl::function::NormalMemberSignature, TypeName)
					EXPECT_NOT_NORMAL_MEMBER_SIGNATURE(MemberJ);
					EXPECT_NOT_NORMAL_MEMBER_SIGNATURE(Callable::VariadicMemberType);
#undef EXPECT_NOT_NORMAL_MEMBER_SIGNATURE
				}

				TEST_F(MemberSignatureParsingTest, TestMemberSignatureParsing) {
#define EXPECT_MEMBER_SIGNATURE(TypeName) \
	EXPECT_TEMPLATE_TRUE(::vkz::mpl::function::MemberFunctionPointer, TypeName)
					EXPECT_MEMBER_SIGNATURE(MemberF);
					EXPECT_MEMBER_SIGNATURE(MemberG);
					EXPECT_MEMBER_SIGNATURE(MemberH);
					EXPECT_MEMBER_SIGNATURE(MemberI);
					EXPECT_MEMBER_SIGNATURE(MemberJ);
					EXPECT_MEMBER_SIGNATURE(MemberOverloadedJ);
					EXPECT_MEMBER_SIGNATURE(MemberK);
					EXPECT_MEMBER_SIGNATURE(Callable::NormalVariadicMemberType);
					EXPECT_MEMBER_SIGNATURE(Callable::VariadicMemberType);
#undef EXPECT_MEMBER_SIGNATURE
#define EXPECT_NOT_MEMBER_SIGNATURE(TypeName) \
	EXPECT_TEMPLATE_FALSE(::vkz::mpl::function::MemberFunctionPointer, TypeName)
					EXPECT_NOT_MEMBER_SIGNATURE(SigNoexcept);
					EXPECT_NOT_MEMBER_SIGNATURE(PtrFun);
					EXPECT_NOT_MEMBER_SIGNATURE(PtrFunNoexcept);
					EXPECT_NOT_MEMBER_SIGNATURE(StdFun);
					EXPECT_NOT_MEMBER_SIGNATURE(Callable);
					EXPECT_NOT_MEMBER_SIGNATURE(Lambda);
					EXPECT_NOT_MEMBER_SIGNATURE(ConstexprLambda);
					EXPECT_NOT_MEMBER_SIGNATURE(QualifiedLambda);
					EXPECT_NOT_MEMBER_SIGNATURE(VariadicPtrFun);
#define MEMBER_RESULT(TypeName) \
	::vkz::mpl::function::member_sig_result_t<TypeName>
					using ResF = MEMBER_RESULT(MemberF);
					using ResG = MEMBER_RESULT(MemberG);
					using ResH = MEMBER_RESULT(MemberH);
					using ResI = MEMBER_RESULT(MemberI);
					using ResJ = MEMBER_RESULT(MemberJ);
					using ResOverloadedJ = MEMBER_RESULT(MemberOverloadedJ);
					using ResK = MEMBER_RESULT(MemberK);
					using ResNVar = MEMBER_RESULT(Callable::NormalVariadicMemberType);
					using ResVar = MEMBER_RESULT(Callable::VariadicMemberType);
#undef MEMBER_RESULT
					EXPECT_TEMPLATE_TRUE(::vkz::mpl::Homogeneous, ResF, ResG, ResH, ResI, ResJ, ResOverloadedJ, ResK);
					EXPECT_TEMPLATE_TRUE(::vkz::mpl::Homogeneous, ResNVar, ResVar);
#define MEMBER_ARGS_PACK(TypeName) \
	::vkz::mpl::function::member_sig_args_pack_t<TypeName>
					using ArgsF = MEMBER_ARGS_PACK(MemberF);
					using ArgsG = MEMBER_ARGS_PACK(MemberG);
					using ArgsH = MEMBER_ARGS_PACK(MemberH);
					using ArgsI = MEMBER_ARGS_PACK(MemberI);
					using ArgsJ = MEMBER_ARGS_PACK(MemberJ);
					using ArgsOverloadedJ = MEMBER_ARGS_PACK(MemberOverloadedJ);
					using ArgsK = MEMBER_ARGS_PACK(MemberK);
					using ArgsNVar = MEMBER_ARGS_PACK(Callable::NormalVariadicMemberType);
					using ArgsVar = MEMBER_ARGS_PACK(Callable::VariadicMemberType);
#undef MEMBER_ARGS_PACK
					EXPECT_TEMPLATE_TRUE(::vkz::mpl::Homogeneous, ArgsF, ArgsG, ArgsH, ArgsI, ArgsJ, ArgsOverloadedJ, ArgsK);
					EXPECT_TEMPLATE_TRUE(::vkz::mpl::Homogeneous, ArgsNVar, ArgsVar);
					EXPECT_TEMPLATE_TRUE(::vkz::mpl::function::NormalVariadicMemberSignature, Callable::NormalVariadicMemberType);
					EXPECT_TEMPLATE_FALSE(::vkz::mpl::function::NormalVariadicMemberSignature, Callable::VariadicMemberType);
				}

				TEST_F(FnTest, TestParsableFuncLike) {
					auto pi = std::make_unique<int>(42);
					using ReferencedLambda = decltype([&pi](int, float) -> void {
						*pi = 1;
					});

					EXPECT_TEMPLATE_TRUE(::vkz::mpl::function::ParsableFuncLike, SigNoexcept);
					EXPECT_TEMPLATE_TRUE(::vkz::mpl::function::ParsableFuncLike, PtrFun);
					EXPECT_TEMPLATE_TRUE(::vkz::mpl::function::ParsableFuncLike, PtrFunNoexcept);
					EXPECT_TEMPLATE_TRUE(::vkz::mpl::function::ParsableFuncLike, StdFun);
					EXPECT_TEMPLATE_TRUE(::vkz::mpl::function::ParsableFuncLike, Callable);
					EXPECT_TEMPLATE_TRUE(::vkz::mpl::function::ParsableFuncLike, Lambda);
					EXPECT_TEMPLATE_TRUE(::vkz::mpl::function::ParsableFuncLike, ConstexprLambda);
					EXPECT_TEMPLATE_TRUE(::vkz::mpl::function::ParsableFuncLike, QualifiedLambda);
					EXPECT_TEMPLATE_TRUE(::vkz::mpl::function::ParsableFuncLike, ReferencedLambda);

					EXPECT_TEMPLATE_TRUE(::vkz::mpl::function::ParsableFuncLike, ResDiffSig);
					EXPECT_TEMPLATE_TRUE(::vkz::mpl::function::ParsableFuncLike, ResDiffPtrFun);
					EXPECT_TEMPLATE_TRUE(::vkz::mpl::function::ParsableFuncLike, ArgsDiffStdFun);
					EXPECT_TEMPLATE_TRUE(::vkz::mpl::function::ParsableFuncLike, ArgsDiffCallable);
					EXPECT_TEMPLATE_TRUE(::vkz::mpl::function::ParsableFuncLike, ResDiffLambda);

					EXPECT_TEMPLATE_FALSE(::vkz::mpl::function::ParsableFuncLike, int);
					EXPECT_TEMPLATE_FALSE(::vkz::mpl::function::ParsableFuncLike, decltype(pi));
				}

				TEST_F(FnTest, TestParsableVariadicFuncLike) {
					EXPECT_TEMPLATE_TRUE(::vkz::mpl::function::ParsableVariadicFuncLike, VariadicPtrFun);
					EXPECT_TEMPLATE_TRUE(::vkz::mpl::function::ParsableVariadicFuncLike, InvalidVariadicPtrFun);
				}

				TEST_F(FnTest, TestSameArgsAs) {
					EXPECT_TEMPLATE_TRUE(::vkz::mpl::function::SameArgsAs, ExpectedSignature, ResDiffSig);
					EXPECT_TEMPLATE_TRUE(::vkz::mpl::function::SameArgsAs, ExpectedSignature, SigNoexcept);
					EXPECT_TEMPLATE_TRUE(::vkz::mpl::function::SameArgsAs, PtrFun, ResDiffPtrFun);
					EXPECT_TEMPLATE_TRUE(::vkz::mpl::function::SameArgsAs, PtrFun, PtrFunNoexcept);
					EXPECT_TEMPLATE_FALSE(::vkz::mpl::function::SameArgsAs, StdFun, ArgsDiffStdFun);
					EXPECT_TEMPLATE_FALSE(::vkz::mpl::function::SameArgsAs, Callable, ArgsDiffCallable);
					EXPECT_TEMPLATE_TRUE(::vkz::mpl::function::SameArgsAs, Lambda, ResDiffLambda);
					EXPECT_TEMPLATE_FALSE(::vkz::mpl::function::SameArgsAs, PtrFun, VariadicPtrFun);
				}

				TEST_F(FnTest, TestSameResultAs) {
					EXPECT_TEMPLATE_FALSE(::vkz::mpl::function::SameResultAs, ExpectedSignature, ResDiffSig);
					EXPECT_TEMPLATE_TRUE(::vkz::mpl::function::SameResultAs, ExpectedSignature, SigNoexcept);
					EXPECT_TEMPLATE_FALSE(::vkz::mpl::function::SameResultAs, PtrFun, ResDiffPtrFun);
					EXPECT_TEMPLATE_TRUE(::vkz::mpl::function::SameResultAs, PtrFun, PtrFunNoexcept);
					EXPECT_TEMPLATE_TRUE(::vkz::mpl::function::SameResultAs, StdFun, ArgsDiffStdFun);
					EXPECT_TEMPLATE_TRUE(::vkz::mpl::function::SameResultAs, Callable, ArgsDiffCallable);
					EXPECT_TEMPLATE_FALSE(::vkz::mpl::function::SameResultAs, Lambda, ResDiffLambda);
					EXPECT_TEMPLATE_TRUE(::vkz::mpl::function::SameResultAs, PtrFun, VariadicPtrFun);
				}

				TEST_F(FnTest, TestSignatureParsing) {
					auto pi = std::make_unique<int>(42);
					using ReferencedLambda = decltype([&pi](int, float) -> void {
						*pi = 1;
					});
					using SigSigNoexcept = ::vkz::mpl::function::parse_signature_from<SigNoexcept>;
					using SigPtrFun = ::vkz::mpl::function::parse_signature_from<PtrFun>;
					using SigPtrFunNoexcept = ::vkz::mpl::function::parse_signature_from<PtrFunNoexcept>;
					using SigStdFun = ::vkz::mpl::function::parse_signature_from<StdFun>;
					using SigCallable = ::vkz::mpl::function::parse_signature_from<Callable>;
					using SigLambda = ::vkz::mpl::function::parse_signature_from<Lambda>;
					using SigConstexprLambda = ::vkz::mpl::function::parse_signature_from<ConstexprLambda>;
					using SigQualifiedLambda = ::vkz::mpl::function::parse_signature_from<QualifiedLambda>;
					using SigReferencedLambda = ::vkz::mpl::function::parse_signature_from<QualifiedLambda>;

					// Verify result of `parse_signature`

					// All arg types equal
					EXPECT_TEMPLATE_TRUE(::vkz::mpl::Homogeneous,
						SigSigNoexcept::args_pack_type,
						SigPtrFun::args_pack_type,
						// TODO:
						// SigPtrFunNoexcept::args_pack_type,
						SigStdFun::args_pack_type,
						SigCallable::args_pack_type,
						SigLambda::args_pack_type,
						SigConstexprLambda::args_pack_type,
						SigQualifiedLambda::args_pack_type,
						SigReferencedLambda::args_pack_type);

					// All result type equal
					EXPECT_TEMPLATE_TRUE(::vkz::mpl::Homogeneous,
						SigSigNoexcept::return_type,
						SigPtrFun::return_type,
						// TODO:
						// SigPtrFunNoexcept::return_type,
						SigStdFun::return_type,
						SigCallable::return_type,
						SigLambda::return_type,
						SigConstexprLambda::return_type,
						SigQualifiedLambda::return_type,
						SigReferencedLambda::return_type
					);
				}

				TEST_F(FnTest, TestFn) {
					auto pi = std::make_unique<int>(42);
					using ReferencedLambda = decltype([&pi](int, float) -> void {
						*pi = 1;
					});

					printType<::vkz::mpl::function::normal_sig_args_pack_t<ExpectedSignature>>("Expected parameter types (Packed): ");
					printType<::vkz::mpl::function::normal_sig_result_t<ExpectedSignature>>("Expected return type: ");
					std::printf("\n");
					// Test `Fn`

#define EXPECT_FN(FunctionType) EXPECT_TEMPLATE_TRUE(::vkz::mpl::function::Fn, FunctionType, ExpectedSignature)
#define EXPECT_NON_FN(FunctionType) EXPECT_TEMPLATE_FALSE(::vkz::mpl::function::Fn, FunctionType, ExpectedSignature)

					// Valid
					EXPECT_FN(SigNoexcept);
					EXPECT_FN(PtrFun);
					EXPECT_FN(PtrFunNoexcept);
					EXPECT_FN(StdFun);
					EXPECT_FN(Callable);
					EXPECT_FN(Lambda);
					EXPECT_FN(ConstexprLambda);
					EXPECT_FN(QualifiedLambda);
					EXPECT_FN(ReferencedLambda);

					// Invalid
					EXPECT_NON_FN(ResDiffSig);
					EXPECT_NON_FN(ResDiffPtrFun);
					EXPECT_NON_FN(ArgsDiffStdFun);
					EXPECT_NON_FN(ArgsDiffCallable);
					EXPECT_NON_FN(ResDiffLambda);

					// Other types
					EXPECT_NON_FN(int);
					EXPECT_NON_FN(decltype(pi));

#undef EXPECT_FN
#undef EXPECT_NON_FN
				}
			}
		}
	}
}
