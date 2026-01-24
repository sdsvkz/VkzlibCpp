#ifndef VKZLIB_MPL_FUNCTION_FNREF_HPP
#define VKZLIB_MPL_FUNCTION_FNREF_HPP

#include <vkzlib/mpl/common/DefaultPack.hpp>
#include <vkzlib/mpl/function/assemble_signature.hpp>
#include <vkzlib/mpl/function/parse/helper/common.hpp>
#include <vkzlib/mpl/function/Fn.hpp>

namespace vkz::mpl::function {
    template<bool CONST, bool MUTABLE, bool NOEX, typename R, typename... Args>
    class _FnRef_impl {
    protected:
        using FType = std::conditional_t<CONST, const void *, void *>;
        using CallType = R (*)(FType, Args...) noexcept(NOEX);

        FType f;
        CallType call;

        template<typename F>
        static R _call(FType _f, Args... args) noexcept(NOEX) {
            using _FType = std::add_pointer_t<std::conditional_t<CONST, std::add_const_t<F>, F>>;
            return (*reinterpret_cast<_FType>(_f))(std::forward<Args>(args)...);
        }
    public:
        constexpr _FnRef_impl() = delete;
        constexpr ~_FnRef_impl() noexcept = default;

        constexpr _FnRef_impl(const _FnRef_impl &other) = delete;
        constexpr _FnRef_impl &operator=(const _FnRef_impl &other) = delete;

        template<Fn<R(Args...)> F>
        constexpr explicit _FnRef_impl(F &&_f) noexcept
            requires (not (CONST and MUTABLE))
            : f(reinterpret_cast<FType>(&_f)), call(_call<std::remove_reference_t<F>>) {}

        constexpr _FnRef_impl(_FnRef_impl &&other) noexcept
            : f(other.f), call(other.call) {}
        constexpr _FnRef_impl &operator=(_FnRef_impl &&other) noexcept {
            f = other.f;
            call = other.call;
            return *this;
        }

        constexpr R operator()(Args... args) const noexcept(NOEX) {
            return call(f, std::forward<Args>(args)...);
        }
    };

    template <typename Signature, bool CONST, bool MUTABLE>
    class FnRef;

    template<typename R, typename... Args, bool CONST, bool MUTABLE>
    class FnRef<R(Args...), CONST, MUTABLE> : public _FnRef_impl<CONST, MUTABLE, false, R, Args...> {
        using BaseClass = _FnRef_impl<CONST, MUTABLE, false, R, Args...>;
    public:
        template<Fn<R(Args...)> F>
        constexpr explicit FnRef(F &&f) noexcept
            : BaseClass(std::forward<F>(f)) {}
    };

    template<typename R, typename... Args, bool CONST, bool MUTABLE>
    class FnRef<R(Args...) noexcept, CONST, MUTABLE> : public _FnRef_impl<CONST, MUTABLE, true, R, Args...> {
        using BaseClass = _FnRef_impl<CONST, MUTABLE, true, R, Args...>;
    public:
        template<Fn<R(Args...)> F>
        constexpr explicit FnRef(F &&f) noexcept
            : BaseClass(std::forward<F>(f)) {}
    };

    template<parse::Parsable F, template<typename...> typename Pack = DefaultPack>
    explicit FnRef(F &&f) -> FnRef<
        assemble_signature_t<parse::result_of_t<F>, parse::args_of_t<F, Pack>>,
        std::is_const_v<std::remove_reference_t<F>>,
        parse::type::MonomorphicFunctor<F> && parse::property::NonCV<F>>;
}

#endif // VKZLIB_MPL_FUNCTION_FNREF_HPP