#ifndef VKZLIB_MPL_TPL_FST_WITHTEMPLATEPARAMS_HPP
#define VKZLIB_MPL_TPL_FST_WITHTEMPLATEPARAMS_HPP

#include <concepts>

#include <vkzlib/mpl/common/Size.hpp>
#include <vkzlib/mpl/common/NPOS.hpp>
#include <vkzlib/mpl/common/ce/findFirstFor.hpp>
#include <vkzlib/mpl/common/pack/unsafe/nth_of.hpp>
#include <vkzlib/mpl/tpl/fst/parse_template_spec.hpp>

namespace vkz::mpl::tpl::fst {
    namespace _detail {
        template<TemplateSpec P>
        consteval Size _indexOfMismatched() {
            return NPOS;
        }

        template<TemplateSpec P, typename T, typename... Ts>
        consteval Size _indexOfMismatched() {
            return ce::findFirstFor<1 + sizeof...(Ts), []<Size I>(SizeConstant<I>) -> bool {
                return not std::same_as<nth_tparam_of_t<I, P>, pack::unsafe::nth_of_t<I, T, Ts...>>;
            }>();
        }

        /**
         * @brief Message to highlight index of mismatched parameter
         */
        inline constexpr char _ARGS_DIFF_INDEX_NOTE[] = "!!!NOTE!!!: Position of mismatched parameter (starts from 0): ";

        /**
         * @brief Message to highlight mismatched parameter type
         */
        inline constexpr char _ARGS_DIFF_TYPE_NOTE[] = "!!!NOTE!!!: When matching: ";

        /**
         * @brief Alias for `std::same_as` to hold additional note message
         */
        template<Size N, const char NOTE[N], typename T, typename U>
        concept _Reason = std::same_as<T, U>;

        template<Size POS, typename P, typename... Ts>
        concept _Retrigger = _Reason<
            sizeof(_ARGS_DIFF_TYPE_NOTE), _ARGS_DIFF_TYPE_NOTE,
            nth_tparam_of_t<POS, P>, pack::unsafe::nth_of_t<POS, Ts...>>;

    }

    template<typename P, typename... Ts>
    concept WithTemplateParams = TemplateSpec<P> &&
        tparam_count_v<P> == sizeof...(Ts) && (
            _detail::_indexOfMismatched<P, Ts...>() == NPOS ||
            _detail::_Retrigger<_detail::_indexOfMismatched<P, Ts...>(), P, Ts...>
        );
}

#endif // VKZLIB_MPL_TPL_FST_WITHTEMPLATEPARAMS_HPP