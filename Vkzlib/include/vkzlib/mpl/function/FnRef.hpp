#ifndef VKZLIB_MPL_FUNCTION_FNREF_HPP
#define VKZLIB_MPL_FUNCTION_FNREF_HPP

#include <vkzlib/mpl/common/DefaultPack.hpp>
#include <vkzlib/mpl/common/tpl/fst/WithTemplateParams.hpp>
#include <vkzlib/mpl/function/parse/helper/common.hpp>
#include <vkzlib/mpl/function/Fn.hpp>

namespace vkz::mpl::function {
    template<parse::Parsable S, typename... Args>
    class FnRef {
        template<template<typename...> typename Pack = DefaultPack>
        using ArgsPack = parse::args_of_t<S, Pack>;
        using R = parse::result_of_t<S>;

        void *f {};
        R (*call)(void *, Args...) {};

        // template<typename F, typename... FArgs, template<typename...> typename Pack = DefaultPack>
        //     requires tpl::fst Fn<F, R(Args...)>




    };
}

#endif // VKZLIB_MPL_FUNCTION_FNREF_HPP