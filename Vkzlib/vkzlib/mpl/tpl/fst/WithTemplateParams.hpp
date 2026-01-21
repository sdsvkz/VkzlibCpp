//
// Created by sdsvkz on 2026-01-21.
//

#ifndef VKZLIB_MPL_TPL_FST_WITHTEMPLATEPARAMS_HPP
#define VKZLIB_MPL_TPL_FST_WITHTEMPLATEPARAMS_HPP

#include <vkzlib/mpl/tpl/fst/parse_template_spec.hpp>

namespace vkz::mpl::tpl::fst {
    namespace _detail {

    }

    /**
     * @brief Template specification filled with exactly `Ts`
     */
    template<typename P, typename... Ts>
    concept WithTemplateParams = TemplateSpec<P> &&
        tparam_count_v<P> == sizeof...(Ts) && (
            sizeof...(Ts) == 0 ||
            ((nth_tparam_of_t<>))
        );
}

#endif // VKZLIB_MPL_TPL_FST_WITHTEMPLATEPARAMS_HPP