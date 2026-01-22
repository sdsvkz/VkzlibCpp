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
        /**
         * @brief Find the index of first mismatched template parameter pair.
         *
         * @return The index, or `NPOS` if all matched
         */
        template<TemplateSpec P, typename... Ts>
        consteval Size _indexOfMismatched() {
            if constexpr (sizeof...(Ts) == 0) {
                return NPOS;
            } else {
                return ce::findFirstFor<sizeof...(Ts), []<Size I>(SizeConstant<I>) -> bool {
                    return not std::same_as<
                        nth_tparam_of_t<I, P>,
                        pack::unsafe::nth_of_t<I, Ts...>>;
                }>();
            }
        }

        /**
         * @brief Message to highlight index of mismatched parameter
         */
        inline constexpr char _ARGS_DIFF_INDEX_NOTE[] = "!!!NOTE!!!: Position of mismatched parameter (starts from 0): ";

        /**
         * @brief Message to highlight mismatched parameter
         */
        inline constexpr char _ARGS_DIFF_TYPE_NOTE[] = "!!!NOTE!!!: When matching: ";

        /**
         * @brief Constraint alias to hold additional note
         */
        template<Size NOTE_SIZE, const char NOTE[NOTE_SIZE], typename T, typename U>
        concept _Reason = std::same_as<T, U>;

        /**
         * @brief Retrigger failed constraint
         */
        template<Size NOTE_SIZE, const char NOTE[NOTE_SIZE], Size POS, typename P, typename... Ts>
        concept _Retrigger = _Reason<
            sizeof(_ARGS_DIFF_TYPE_NOTE), _ARGS_DIFF_TYPE_NOTE,
            nth_tparam_of_t<POS, P>, pack::unsafe::nth_of_t<POS, Ts...>>;

        template<Size POS, typename P, typename... Ts>
        concept _SameTemplateParams =
            POS == NPOS ||
            _Retrigger<
                sizeof(_ARGS_DIFF_INDEX_NOTE), _ARGS_DIFF_INDEX_NOTE,
                POS, P, Ts...>;
    }

    /**
     * @brief Template specialization `P` with template parameters `Ts`
     */
    template<typename P, typename... Ts>
    concept WithTemplateParams = TemplateSpec<P> &&
        tparam_count_v<P> == sizeof...(Ts) &&
        _detail::_SameTemplateParams<_detail::_indexOfMismatched<P, Ts...>(), P, Ts...>;
}

#endif // VKZLIB_MPL_TPL_FST_WITHTEMPLATEPARAMS_HPP