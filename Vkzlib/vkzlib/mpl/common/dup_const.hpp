#ifndef VKZLIB_MPL_COMMON_DUP_CONST_HPP
#define VKZLIB_MPL_COMMON_DUP_CONST_HPP

#include <type_traits>

namespace vkz::mpl {
    template<typename From, typename To>
    struct dup_const {
    private:
        using _ConstRemoved = std::remove_const_t<To>;
    public:
        using type = std::conditional_t<std::is_const_v<From>,
            std::add_const_t<_ConstRemoved>,
            _ConstRemoved>;
    };

    template<typename From, typename To>
    using dup_const_t = dup_const<From, To>::type;
}

#endif // VKZLIB_MPL_COMMON_DUP_CONST_HPP