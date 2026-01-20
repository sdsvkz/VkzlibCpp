#ifndef VKZLIB_MPL_FUNCTION_SAMEARGSAS_HPP
#define VKZLIB_MPL_FUNCTION_SAMEARGSAS_HPP

#include <limits>
#include <utility>
#include <type_traits>
#include <concepts>

#include <vkzlib/mpl/function/internal/DefaultPack.hpp>
#include <vkzlib/mpl/function/parse/helper/common.hpp>
#include <vkzlib/mpl/function/parse/property/concepts.hpp>
#include <vkzlib/mpl/tpl/fst/parse_template_spec.hpp>

namespace vkz::mpl::function {
    namespace _detail {
        /**
         * @brief Find the index of the first mismatched parameter
         *
         * @return Index of mismatched parameter, or `std::numeric_limits<std::size_t>::max()` if not found
         */
        template<typename F, typename G,
            template <typename...> typename FPack,
            template <typename...> typename GPack>
        consteval decltype(auto) _findIndexOfMismatched() {
            constexpr auto MAX = std::numeric_limits<std::size_t>::max();
            return []<std::size_t... Is>(std::index_sequence<Is...>) {
                return std::min({ MAX, (!std::same_as<
                    tpl::fst::nth_tparam_of_t<Is, parse::args_of_t<F, FPack>>,
                    tpl::fst::nth_tparam_of_t<Is, parse::args_of_t<G, GPack>>
                > ? Is : MAX)... });
            }(std::make_index_sequence<tpl::fst::tparam_count_v<parse::args_of_t<F, FPack>>>());
        }

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
            std::size_t NOTE_SIZE,
            const char NOTE[NOTE_SIZE],
            std::size_t Position>
        struct _RetrieveMismatchedPair {
            using fst = tpl::fst::nth_tparam_of_t<Position, parse::args_of_t<F, FPack>>;
            using snd = tpl::fst::nth_tparam_of_t<Position, parse::args_of_t<G, GPack>>;
        };

        /**
         * @brief Message to highlight index of mismatched parameter
         */
        inline constexpr char ARGS_DIFF_INDEX_NOTE[] = "!!!NOTE!!!: Position of mismatched parameter (starts from 0): ";

        /**
         * @brief All matched case
         */
        template<typename F, typename G,
            template <typename...> typename FPack,
            template <typename...> typename GPack,
            std::size_t NOTE_SIZE,
            const char NOTE[NOTE_SIZE]>
        struct _RetrieveMismatchedPair<F, G, FPack, GPack, NOTE_SIZE, NOTE, std::numeric_limits<std::size_t>::max()> {
            using fst = std::void_t<>;
            using snd = std::void_t<>;
        };

        inline constexpr char ARGS_DIFF_TYPE_NOTE[] = "!!!NOTE!!!: When matching: ";

        /**
         * @brief Alias for `std::same_as` to hold additional note message
         */
        template<std::size_t N, const char NOTE[N], typename T, typename U>
        concept Match = std::same_as<T, U>;

        template<typename F, typename G,
            template <typename...> typename FPack,
            template <typename...> typename GPack,
            typename FailedPair = _RetrieveMismatchedPair<
                F, G, FPack, GPack,
                sizeof(ARGS_DIFF_INDEX_NOTE), ARGS_DIFF_INDEX_NOTE,
                _detail::_findIndexOfMismatched<F, G, FPack, GPack>()>>
        concept _SameArgs = Match<sizeof(ARGS_DIFF_TYPE_NOTE), ARGS_DIFF_TYPE_NOTE, typename FailedPair::fst, typename FailedPair::snd>;
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