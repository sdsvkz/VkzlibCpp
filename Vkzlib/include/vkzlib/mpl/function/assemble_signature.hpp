#ifndef VKZLIB_MPL_FUNCTION_ASSEMBLE_SIGNATURE_HPP
#define VKZLIB_MPL_FUNCTION_ASSEMBLE_SIGNATURE_HPP

#include <vkzlib/mpl/preprocessor/signature/map_syntax_product.hpp>
#include <vkzlib/mpl/preprocessor/signature/tag_to_enum.hpp>
#include <vkzlib/mpl/preprocessor/signature/untag.hpp>
#include <vkzlib/mpl/function/property/enums.hpp>

namespace vkz::mpl::function {
    template<typename R, typename ArgsPack,
        property::ExceptionQualifier Ex = property::ExceptionQualifier::None,
        property::CVQualifier CV = property::CVQualifier::None,
        property::RefQualifier REF = property::RefQualifier::None,
        property::VariadicParam VAR = property::VariadicParam::None>
    struct assemble_signature;

#define VKZLIB_DEFINE_ASSEMBLE_SIGNATURE(                                           \
    VAR_TAG, CV_TAG, REF_TAG, NOEX_TAG                                              \
)                                                                                   \
    template<typename R, typename... Args,                                          \
        template<typename...> typename Pack>                                        \
    struct assemble_signature<R, Pack<Args...>,                                     \
        property::ExceptionQualifier::VKZLIB_PP_SIGNATURE_TAG_TO_ENUM(NOEX_TAG),    \
        property::CVQualifier::VKZLIB_PP_SIGNATURE_TAG_TO_ENUM(CV_TAG),             \
        property::RefQualifier::VKZLIB_PP_SIGNATURE_TAG_TO_ENUM(REF_TAG),           \
        property::VariadicParam::VKZLIB_PP_SIGNATURE_TAG_TO_ENUM(VAR_TAG)>          \
    {                                                                               \
        using type = R(Args... VKZLIB_PP_SIGNATURE_UNTAG(VAR_TAG))                  \
            VKZLIB_PP_SIGNATURE_UNTAG(CV_TAG)                                       \
            VKZLIB_PP_SIGNATURE_UNTAG(REF_TAG)                                      \
            VKZLIB_PP_SIGNATURE_UNTAG(NOEX_TAG);                                    \
    };

    VKZLIB_PP_SIGNATURE_MAP_SYNTAX_PRODUCT(VKZLIB_DEFINE_ASSEMBLE_SIGNATURE)

    template<typename R, typename ArgsPack,
        property::ExceptionQualifier Ex = property::ExceptionQualifier::None,
        property::CVQualifier CV = property::CVQualifier::None,
        property::RefQualifier REF = property::RefQualifier::None,
        property::VariadicParam VAR = property::VariadicParam::None>
    using assemble_signature_t = assemble_signature<R, ArgsPack, Ex, CV, REF, VAR>::type;
}

#endif // VKZLIB_MPL_FUNCTION_ASSEMBLE_SIGNATURE_HPP