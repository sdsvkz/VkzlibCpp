#ifndef VKZLIB_MPL_FUNCTION_SAMEARGSAS_HPP
#define VKZLIB_MPL_FUNCTION_SAMEARGSAS_HPP

#include <type_traits>
#include <concepts>

#include <vkzlib/mpl/common/Size.hpp>
#include <vkzlib/mpl/common/NPOS.hpp>
#include <vkzlib/mpl/common/DefaultPack.hpp>
#include <vkzlib/mpl/common/ce/findFirstFor.hpp>
#include <vkzlib/mpl/common/tpl/fst/parse_template_spec.hpp>
#include <vkzlib/mpl/function/parse/helper/common.hpp>
#include <vkzlib/mpl/function/parse/property/concepts.hpp>

namespace vkz::mpl::function {
    namespace _detail {
        /**
         * @brief Find the index of the first mismatched parameter pair
         *
         * @return The index, or `NPOS` if all matched
         */
        template<typename F, typename G,
            template <typename...> typename FPack,
            template <typename...> typename GPack>
        consteval Size _indexOfMismatchedParam() {
            constexpr auto TPARAM_COUNT = tpl::fst::tparam_count_v<parse::args_of_t<F, FPack>>;
            if constexpr (TPARAM_COUNT == 0) {
                return NPOS;
            } else {
                return ce::findFirstFor<TPARAM_COUNT, []<Size I>
                    (SizeConstant<I>) -> bool {
                        return not std::same_as<
                            tpl::fst::nth_tparam_of_t<I, parse::args_of_t<F, FPack>>,
                            tpl::fst::nth_tparam_of_t<I, parse::args_of_t<G, GPack>>>;
                    }
                >();
            }
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
         * @brief Constraint alias to hold additional note
         */
        template<Size NOTE_SIZE, const char NOTE[NOTE_SIZE], typename T, typename U>
        concept _Reason = std::same_as<T, U>;

        /**
         * @brief Retrigger failed constraint
         */
        template<Size NOTE_SIZE, const char NOTE[NOTE_SIZE], Size POS,
            typename F, typename G,
            template <typename...> typename FPack,
            template <typename...> typename GPack>
        concept _Retrigger = _Reason<
            sizeof(_ARGS_DIFF_TYPE_NOTE), _ARGS_DIFF_TYPE_NOTE,
            tpl::fst::nth_tparam_of_t<POS, parse::args_of_t<F, FPack>>,
            tpl::fst::nth_tparam_of_t<POS, parse::args_of_t<G, GPack>>>;

        template<Size POS, typename F, typename G,
            template <typename...> typename FPack,
            template <typename...> typename GPack>
        concept _SameArgs =
            POS == NPOS ||
            _Retrigger<
                sizeof(_ARGS_DIFF_INDEX_NOTE), _ARGS_DIFF_INDEX_NOTE,
                POS, F, G, FPack, GPack>;
    }

    /**
     * @brief Parsable function type with the same parameter list as `G`, including C variadic parameter (`...`).
     *
     * @tparam FPack Container for storing types of arguments, for `F`
     * @tparam GPack Same as `FPack`, but for `G`
     */
    template<typename F, typename G,
        template <typename...> typename FPack = DefaultPack,
        template <typename...> typename GPack = DefaultPack>
    concept SameArgsAs =
        parse::Parsable<F> &&
        parse::Parsable<G> &&
        parse::property::Variadic<F> == parse::property::Variadic<G> &&
        tpl::fst::tparam_count_v<parse::args_of_t<F, FPack>> == tpl::fst::tparam_count_v<parse::args_of_t<G, GPack>> &&
        _detail::_SameArgs<_detail::_indexOfMismatchedParam<F, G, FPack, GPack>(), F, G, FPack, GPack>;
}

#endif // VKZLIB_MPL_FUNCTION_SAMEARGSAS_HPP