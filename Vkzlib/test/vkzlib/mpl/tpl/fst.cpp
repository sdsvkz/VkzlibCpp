#include <gtest/gtest.h>

#include <vkzlib/mpl/tpl/fst.hpp>

#include <test/common.h>

namespace MplTplFstTest {
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
