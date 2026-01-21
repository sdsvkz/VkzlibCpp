#ifndef VKZLIB_MPL_FUNCTION_MONOMORPHICFUNCTOR_HPP
#define VKZLIB_MPL_FUNCTION_MONOMORPHICFUNCTOR_HPP

#include <utility>

#include "vkzlib/mpl/common/Class.hpp"

namespace vkz::mpl::function {
    /**
    * @brief Classes with `operator()` but not overloaded
    */
    template<class T>
    concept MonomorphicFunctor = Class<T> && requires {
        { std::declval<decltype(&T::operator())>() };
    };
}

#endif // VKZLIB_MPL_FUNCTION_MONOMORPHICFUNCTOR_HPP