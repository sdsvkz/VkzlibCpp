#include <gtest/gtest.h>

#include <vkzlib/mpl/tpl/fst.hpp>

#include <common.hpp>

namespace MplTplFstTest {
    using namespace ::vkz::mpl;
    using namespace ::vkz::mpl::tpl::fst;

    TEST(TplFstTest, TestIsSameTemplate) {
        constexpr bool e1 = is_same_template_v<std::vector, std::vector>;
        EXPECT_TRUE(e1);
        constexpr bool e2 = is_same_template_v<std::tuple, std::tuple>;
        EXPECT_TRUE(e2);
        constexpr bool e3 = is_same_template_v<std::vector, std::tuple>;
        EXPECT_FALSE(e3);
    }

    TEST(TplFstTest, TestFirstOrderTemplateSpec) {
        EXPECT_TEMPLATE(TRUE, TemplateSpec, std::vector<int>);
        EXPECT_TEMPLATE(TRUE, TemplateSpec, std::tuple<>);
        EXPECT_TEMPLATE(TRUE, TemplateSpec, std::tuple<int, float>);

        EXPECT_TEMPLATE(FALSE, TemplateSpec, std::true_type);
        EXPECT_TEMPLATE(FALSE, TemplateSpec, std::false_type);
    }

    TEST(TplFstTest, TestTparamCount) {
        using EmptyTuple = std::tuple<>;
        using TupleA = std::tuple<int, float>;
        using VectorA = std::vector<int>;

        EXPECT_EQ(tparam_count_v<EmptyTuple>, 0);
        EXPECT_EQ(tparam_count_v<TupleA>, 2);
        EXPECT_EQ(tparam_count_v<VectorA>, 2);
    }

    TEST(TplFstTest, TestWithTemplateParam) {
        using EmptyTuple = std::tuple<>;
        using TupleA = std::tuple<int, float>;
        using VectorA = std::vector<int>;
#define EXPECT_WITH_TEMPLATE_PARAMS(BOOL, ...) EXPECT_TEMPLATE(BOOL, WithTemplateParams, __VA_ARGS__)
        EXPECT_WITH_TEMPLATE_PARAMS(TRUE, EmptyTuple);
        EXPECT_WITH_TEMPLATE_PARAMS(TRUE, TupleA, int, float);
        EXPECT_WITH_TEMPLATE_PARAMS(TRUE, VectorA, int, std::allocator<int>);

        EXPECT_WITH_TEMPLATE_PARAMS(FALSE, EmptyTuple, int, float);
        EXPECT_WITH_TEMPLATE_PARAMS(FALSE, TupleA, int, int);
        EXPECT_WITH_TEMPLATE_PARAMS(FALSE, VectorA, int);
#undef EXPECT_WITH_TEMPLATE_PARAMS
    }
}
