#include <concepts>
#include <type_traits>

#include <gtest/gtest.h>

#include <vkzlib/mpl/pack.hpp>

#include <common.h>

namespace MplPackTest {
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
