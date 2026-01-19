#ifndef VKZLIB_MPL_COMMON_DUP_REFERENCE_HPP
#define VKZLIB_MPL_COMMON_DUP_REFERENCE_HPP

#include <type_traits>

namespace vkz::mpl {
    template<typename From, typename To>
    struct dup_reference {
    private:
        using _RefRemoved = std::remove_reference_t<To>;
    public:
        using type =
            std::conditional_t<std::is_lvalue_reference_v<From>,
                std::add_lvalue_reference_t<_RefRemoved>,
                std::conditional_t<std::is_rvalue_reference_v<From>,
                    std::add_rvalue_reference_t<_RefRemoved>,
                    _RefRemoved>>;
    };

    template<typename From, typename To>
    using dup_reference_t = dup_reference<From, To>::type;
}

#endif // VKZLIB_MPL_COMMON_DUP_REFERENCE_HPP