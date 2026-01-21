#ifndef VKZLIB_MPL_FUNCTION_SAMERESULTAS_HPP
#define VKZLIB_MPL_FUNCTION_SAMERESULTAS_HPP

#include <concepts>

#include <vkzlib/mpl/function/parse/helper/common.hpp>

namespace vkz::mpl::function {
    /**
     * @brief Parsable function type with the same return type as `F`.
     */
    template<typename F, typename G>
    concept SameResultAs =
        parse::Parsable<F> &&
        parse::Parsable<G> &&
        std::same_as<parse::result_of_t<F>, parse::result_of_t<G>>;
}

#endif // VKZLIB_MPL_FUNCTION_SAMERESULTAS_HPP