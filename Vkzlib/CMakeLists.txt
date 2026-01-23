include(GNUInstallDirs)

set(VKZLIB_MPL_PREPROCESSOR_HEADERS
    "include/vkzlib/mpl/preprocessor.hpp"

    "include/vkzlib/mpl/preprocessor/and.hpp"
    "include/vkzlib/mpl/preprocessor/cat.hpp"
    "include/vkzlib/mpl/preprocessor/conditional.hpp"
    "include/vkzlib/mpl/preprocessor/const.hpp"
    "include/vkzlib/mpl/preprocessor/empty.hpp"
    "include/vkzlib/mpl/preprocessor/empty_tuple.hpp"
    "include/vkzlib/mpl/preprocessor/expand_if.hpp"
    "include/vkzlib/mpl/preprocessor/identity.hpp"
    "include/vkzlib/mpl/preprocessor/is_empty.hpp"
    "include/vkzlib/mpl/preprocessor/is_zero.hpp"
    "include/vkzlib/mpl/preprocessor/not.hpp"
    "include/vkzlib/mpl/preprocessor/or.hpp"
    "include/vkzlib/mpl/preprocessor/pick_second_or.hpp"
    "include/vkzlib/mpl/preprocessor/signature.hpp"
    "include/vkzlib/mpl/preprocessor/unpack_tuple.hpp"
    "include/vkzlib/mpl/preprocessor/va_opt.hpp"

    "include/vkzlib/mpl/preprocessor/signature/is_tag.hpp"
    "include/vkzlib/mpl/preprocessor/signature/map_syntax_product.hpp"
    "include/vkzlib/mpl/preprocessor/signature/tag.hpp"
    "include/vkzlib/mpl/preprocessor/signature/tag_to_enum.hpp"
    "include/vkzlib/mpl/preprocessor/signature/untag.hpp"
)

set(VKZLIB_HEADERS
    ${VKZLIB_MPL_PREPROCESSOR_HEADERS}

    "include/vkzlib/mpl.hpp"

    "include/vkzlib/mpl/ce.hpp"
    "include/vkzlib/mpl/common.hpp"
    "include/vkzlib/mpl/function.hpp"
    "include/vkzlib/mpl/pack.hpp"
    "include/vkzlib/mpl/tpl.hpp"

    "include/vkzlib/mpl/common/AnyOf.hpp"
    "include/vkzlib/mpl/common/Class.hpp"
    "include/vkzlib/mpl/common/conditionalCall.hpp"
    "include/vkzlib/mpl/common/CString.hpp"
    "include/vkzlib/mpl/common/DefaultPack.hpp"
    "include/vkzlib/mpl/common/dup_const.hpp"
    "include/vkzlib/mpl/common/dup_cvref.hpp"
    "include/vkzlib/mpl/common/dup_reference.hpp"
    "include/vkzlib/mpl/common/dup_volatile.hpp"
    "include/vkzlib/mpl/common/HasCommonType.hpp"
    "include/vkzlib/mpl/common/Homogeneous.hpp"
    "include/vkzlib/mpl/common/NPOS.hpp"
    "include/vkzlib/mpl/common/Size.hpp"

    "include/vkzlib/mpl/common/ce/findFirstFor.hpp"
    "include/vkzlib/mpl/common/ce/foldl.hpp"
    "include/vkzlib/mpl/common/ce/min.hpp"
    "include/vkzlib/mpl/common/ce/unroll.hpp"

    "include/vkzlib/mpl/common/pack/is_empty_pack.hpp"
    "include/vkzlib/mpl/common/pack/nth_of.hpp"
    "include/vkzlib/mpl/common/pack/Pack.hpp"

    "include/vkzlib/mpl/common/pack/unsafe/nth_of.hpp"
    "include/vkzlib/mpl/common/pack/unsafe/uncons.hpp"

    "include/vkzlib/mpl/common/tpl/fst/parse_template_spec.hpp"
    "include/vkzlib/mpl/common/tpl/fst/WithTemplateParams.hpp"

    "include/vkzlib/mpl/function/Fn.hpp"
    "include/vkzlib/mpl/function/FnRef.hpp"
    "include/vkzlib/mpl/function/MonomorphicFunctor.hpp"
    "include/vkzlib/mpl/function/parse.hpp"
    "include/vkzlib/mpl/function/property.hpp"
    "include/vkzlib/mpl/function/SameArgsAs.hpp"
    "include/vkzlib/mpl/function/SameResultAs.hpp"

    "include/vkzlib/mpl/function/parse/helper.hpp"
    "include/vkzlib/mpl/function/parse/parse.hpp"
    "include/vkzlib/mpl/function/parse/property.hpp"
    "include/vkzlib/mpl/function/parse/type.hpp"

    "include/vkzlib/mpl/function/parse/helper/common.hpp"
    "include/vkzlib/mpl/function/parse/helper/member_function_pointer.hpp"
    "include/vkzlib/mpl/function/parse/helper/stl_function_like.hpp"

    "include/vkzlib/mpl/function/parse/property/concepts.hpp"

    "include/vkzlib/mpl/function/parse/type/enums.hpp"
    "include/vkzlib/mpl/function/parse/type/concepts.hpp"

    "include/vkzlib/mpl/function/property/enums.hpp"

    "include/vkzlib/mpl/tpl/fst.hpp"

    "include/vkzlib/mpl/tpl/fst/SameTemplateAs.hpp"
)

add_library(vkzlib INTERFACE)

target_include_directories(vkzlib INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_sources(vkzlib
    INTERFACE
    FILE_SET HEADERS
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
    FILES ${VKZLIB_HEADERS}
)

target_compile_features(vkzlib INTERFACE cxx_std_20)

target_compile_options(vkzlib INTERFACE
    $<$<CXX_COMPILER_ID:MSVC>:/Zc:preprocessor>
)

if (BUILD_TESTING)
  add_subdirectory("test")
endif ()

install(
    TARGETS vkzlib
    EXPORT vkzlibTargets
    FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    EXPORT vkzlibTargets
    NAMESPACE vkzlib::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/vkzlib
)
