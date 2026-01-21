#include <gtest/gtest.h>

#include <vkzlib/mpl/common.hpp>

#include <common.h>

namespace MplCommonTest {
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

namespace MplCommonCeTest {
	using namespace ::vkz::mpl::ce;

	TEST(MplCommonCeTest, TestFindFirstFor) {
		constexpr auto V = findFirstFor<10, []<std::size_t I>
			(std::integral_constant<std::size_t, I>) -> bool {
				return std::greater<std::size_t>()(I * 2, 5);
			}
		>();
		EXPECT_EQ(V, 3);
	}

	TEST(MplCommonCeTest, TestFold) {
		constexpr auto V = foldl<[](const int acc, const int x) -> int {
			return acc * x + x;
		}>(1, 3, 5, 2, 4);
		constexpr auto E1 = 1 * 3 + 3;
		constexpr auto E2 = E1 * 5 + 5;
		constexpr auto E3 = E2 * 2 + 2;
		constexpr auto E4 = E3 * 4 + 4;
		EXPECT_EQ(V, E4);
	}

	TEST(MplCommonCeTest, TestUnroll) {
		constexpr std::size_t INITIAL = 0;
		constexpr auto V = unroll<10, []<std::size_t I>
			(const std::size_t acc, std::integral_constant<std::size_t, I>) {
				return acc + I;
			}
		>(INITIAL);
		EXPECT_EQ(V, 0 + 1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9);
	}
}
