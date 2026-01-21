#ifndef VKZLIB_MPL_COMMON_DUP_VOLATILE_HPP
#define VKZLIB_MPL_COMMON_DUP_VOLATILE_HPP

#include <type_traits>

namespace vkz::mpl {
    template<typename From, typename To>
    struct dup_volatile {
    private:
        using _VolatileRemoved = std::remove_volatile_t<To>;
    public:
        using type = std::conditional_t<std::is_volatile_v<From>,
            std::add_volatile_t<_VolatileRemoved>,
            _VolatileRemoved>;
    };

    template<typename From, typename To>
    using dup_volatile_t = dup_volatile<From, To>::type;
}

#endif // VKZLIB_MPL_COMMON_DUP_VOLATILE_HPP