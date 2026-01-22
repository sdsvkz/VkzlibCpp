#ifndef VKZLIB_MPL_FUNCTION_SAMEARGSAS_HPP
#define VKZLIB_MPL_FUNCTION_SAMEARGSAS_HPP

#include <type_traits>
#include <concepts>

#include <vkzlib/mpl/common/Size.hpp>
#include <vkzlib/mpl/common/NPOS.hpp>
#include <vkzlib/mpl/common/DefaultPack.hpp>
#include <vkzlib/mpl/common/ce/findFirstFor.hpp>
#include <vkzlib/mpl/function/parse/helper/common.hpp>
#include <vkzlib/mpl/function/parse/property/concepts.hpp>
#include <vkzlib/mpl/tpl/fst/parse_template_spec.hpp>

namespace vkz::mpl::function {
    namespace _detail {
        /**
         * @brief Find the index of the first mismatched parameter
         *
         * @return Index of mismatched parameter, or `std::numeric_limits<Size>::max()` if not found
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
                        return !std::same_as<
                            tpl::fst::nth_tparam_of_t<I, parse::args_of_t<F, FPack>>,
                            tpl::fst::nth_tparam_of_t<I, parse::args_of_t<G, GPack>>
                        >;
                    }
                >();
            }
        }

        /**
         * @brief Wrapper of `tpl::fst::nth_tparam_of_t`. SHUT THE FUCK UP MSVC
         *
         * @note If I don't do this, MSVC will instantiate `nth_tparam_of_t` with `_NPOS` and blows me up.
         *       I mean what the fuck are you thinking about to instantiate a unreached template?
         */
        template<Size POS, typename Args>
        struct _Nth {
            using type = tpl::fst::nth_tparam_of_t<POS, Args>;
        };

        template<typename Args>
        struct _Nth<NPOS, Args> {
            using type = std::void_t<>;
        };

        /**
         * @brief Get the first pair of mismatched parameter.
         *
         * Forward them to `std::same_as` to trigger constraint message
         *
         * If all matched, gives `fst` and `snd` so that `std::same_as<fst, snd>` is always true
         */
        template<typename F, typename G,
            template <typename...> typename FPack,
            template <typename...> typename GPack,
            Size NOTE_SIZE,
            const char NOTE[NOTE_SIZE],
            Size POS>
        struct _RetrieveMismatchedPair {
            using fst = _Nth<POS, parse::args_of_t<F, FPack>>::type;
            using snd = _Nth<POS, parse::args_of_t<G, GPack>>::type;
        };

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
        concept _Match = std::same_as<T, U>;

        template<typename F, typename G,
            template <typename...> typename FPack,
            template <typename...> typename GPack,
            typename MismatchedPair = _RetrieveMismatchedPair<
                F, G, FPack, GPack,
                sizeof(_ARGS_DIFF_INDEX_NOTE), _ARGS_DIFF_INDEX_NOTE,
                _detail::_indexOfMismatchedParam<F, G, FPack, GPack>()>>
        concept _SameArgs = _Match<
            sizeof(_ARGS_DIFF_TYPE_NOTE), _ARGS_DIFF_TYPE_NOTE,
            typename MismatchedPair::fst, typename MismatchedPair::snd>;
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
        _detail::_SameArgs<F, G, FPack, GPack>;
}

#endif // VKZLIB_MPL_FUNCTION_SAMEARGSAS_HPP