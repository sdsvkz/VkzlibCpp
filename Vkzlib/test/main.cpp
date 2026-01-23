#include <type_traits>
#include <functional>
#include <memory>

#include <gtest/gtest.h>

#include <vkzlib/mpl.hpp>

#include <common.hpp>

using namespace ::vkz::mpl;

constexpr float f(const int x, const float y) {
    return static_cast<float>(x) + y;
}

TEST(UseTest, TestMain) {
    using namespace function;

    int i = 0;

    auto g = [i](const int x, const float y) mutable {
        ++i;
        std::cout << i << std::endl;
        return static_cast<float>(x) + y;
    };
    const std::function h {g};

    const auto refF = FnRef(f);
    const auto refG = FnRef(g);
    const auto refH = FnRef(h);
    const auto resF = refF(114, 51.4);
    const auto resG = refG(1, 1.4);
    const auto resH = refH(5, 1.4);
    std::cout << resF << std::endl;
    std::cout << resG << std::endl;
    std::cout << resH << std::endl;
}
