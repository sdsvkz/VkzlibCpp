#ifndef VKZLIB_MPL_FUNCTION_FNREF_HPP
#define VKZLIB_MPL_FUNCTION_FNREF_HPP

#include <vkzlib/mpl/function/internal/DefaultPack.hpp>
#include <vkzlib/mpl/function/parse/helper.hpp>
#include <vkzlib/mpl/function/Fn.hpp>

namespace vkz::mpl::function {
    // template<parse::Parsable S, typename... Args>
    // class FnRef {
    //     using R = parse::result_of_t<S>;
    //
    //     void *f {};
    //     R (*call)(void *, Args...) {};
    //
    //     template<typename F, typename... FArgs>
    //         requires Fn<F, R(Args...)>
    //
    // };
}

#endif // VKZLIB_MPL_FUNCTION_FNREF_HPP