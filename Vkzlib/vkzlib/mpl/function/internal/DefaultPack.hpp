#ifndef VKZLIB_MPL_FUNCTION_INTERNAL_DEFAULTPACK_HPP
#define VKZLIB_MPL_FUNCTION_INTERNAL_DEFAULTPACK_HPP

#include <vkzlib/mpl/pack/Pack.hpp>

namespace vkz::mpl::function {
    /**
    * @brief Default template type used to store template parameter pack
    * @tparam Ts Template parameter pack to be stored
    */
    template<typename... Ts>
    using DefaultPack = pack::Pack<Ts...>;
}

#endif // VKZLIB_MPL_FUNCTION_INTERNAL_DEFAULTPACK_HPP