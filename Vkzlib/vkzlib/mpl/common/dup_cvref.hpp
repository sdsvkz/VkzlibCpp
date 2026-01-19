#ifndef VKZLIB_DUP_CVREF_HPP
#define VKZLIB_DUP_CVREF_HPP

#include <type_traits>

#include <vkzlib/mpl/common/dup_reference.hpp>

namespace vkz::mpl {
    template<typename From, typename To>
    struct dup_cvref {
    private:
        using _Base = std::remove_cvref_t<To>;
        using _ConstHandled = std::conditional_t<
            std::is_const_v<std::remove_reference_t<From>>,
            std::add_const_t<_Base>,
            _Base>;
        using _CVHandled = std::conditional_t<
            std::is_volatile_v<std::remove_reference_t<From>>,
            std::add_volatile_t<_ConstHandled>,
            _ConstHandled>;
    public:
        using type = dup_reference_t<From, _CVHandled>;
    };

    template<typename From, typename To>
    using dup_cvref_t = dup_cvref<From, To>::type;
}

#endif //VKZLIB_DUP_CVREF_HPP