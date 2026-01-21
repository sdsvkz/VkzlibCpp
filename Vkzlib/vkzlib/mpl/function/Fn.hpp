#ifndef VKZLIB_MPL_FUNCTION_FN_HPP
#define VKZLIB_MPL_FUNCTION_FN_HPP

#include <vkzlib/mpl/function/SameArgsAs.hpp>
#include <vkzlib/mpl/function/SameResultAs.hpp>
#include <vkzlib/mpl/function/internal/DefaultPack.hpp>
#include <vkzlib/mpl/function/parse/type/concepts.hpp>

namespace vkz::mpl::function {
    /**
     * @brief Direct invocable type with same signature as `G`
     */
    template<typename F, typename G, template <typename...> typename Pack = internal::DefaultPack>
    concept Fn =
        parse::type::DirectInvocable<F> &&
        SameResultAs<F, G> &&
        SameArgsAs<F, G, Pack>;
}

#endif // VKZLIB_MPL_FUNCTION_FN_HPP