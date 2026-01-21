#include <functional>
#include <memory>

#include <gtest/gtest.h>

#include <vkzlib/mpl.hpp>
#include <vkzlib/mpl/common/ce/findFirstFor.hpp>

#include <test/common.h>

using namespace ::vkz;

TEST(UseTest, TestMain) {
	using mpl::ce::findFirstFor;
	using mpl::function::Fn;
	using namespace mpl::function;
	using namespace mpl::function::internal;
	using namespace mpl::function::_detail;
	[[maybe_unused]] auto f = [](int x, float y) {
		std::printf("%d, %.2f", x, y);
	};
	using FunctionTypeA = decltype(f);
	using SignatureB = void(int, float);

	constexpr auto N1 = findFirstFor<10, []<std::size_t I>
		(std::integral_constant<std::size_t, I>) -> bool {
			return I % 3 == 2;
		}
	>();
	std::cout << N1 << std::endl;

	static_assert(Fn<FunctionTypeA, SignatureB>);
}
