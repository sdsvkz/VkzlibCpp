# CMakeList.txt : CMake project for VkzlibCpp, include source and define
# project specific logic here.
#

add_library(vkzlib INTERFACE
    # vkzlib/internal/
    "vkzlib/internal/begin_shareddefs.h"
    "vkzlib/internal/end_shareddefs.h"
    # vkzlib/
    "vkzlib/mpl.hpp"
    # vkzlib/mpl/
    "vkzlib/mpl/common.hpp"
    "vkzlib/mpl/function.hpp"
    "vkzlib/mpl/pack.hpp"
    "vkzlib/mpl/preprocessor.hpp"
    "vkzlib/mpl/tpl.hpp"
    # vkzlib/mpl/common/
    "vkzlib/mpl/common/AnyOf.hpp"
    "vkzlib/mpl/common/Class.hpp"
    "vkzlib/mpl/common/conditionalCall.hpp"
    "vkzlib/mpl/common/CString.hpp"
    "vkzlib/mpl/common/dup_const.hpp"
    "vkzlib/mpl/common/dup_cvref.hpp"
    "vkzlib/mpl/common/dup_reference.hpp"
    "vkzlib/mpl/common/dup_volatile.hpp"
    "vkzlib/mpl/common/HasCommonType.hpp"
    "vkzlib/mpl/common/Homogeneous.hpp"
    # vkzlib/mpl/function/
    "vkzlib/mpl/function/Fn.hpp"
    "vkzlib/mpl/function/MonomorphicFunctor.hpp"
    "vkzlib/mpl/function/parse.hpp"
    "vkzlib/mpl/function/property.hpp"
    "vkzlib/mpl/function/SameArgsAs.hpp"
    "vkzlib/mpl/function/SameResultAs.hpp"
    # vkzlib/mpl/function/internal/
    "vkzlib/mpl/function/internal/DefaultPack.hpp"
    # vkzlib/mpl/function/parse/
    "vkzlib/mpl/function/parse/helper.hpp"
    "vkzlib/mpl/function/parse/parse.hpp"
    "vkzlib/mpl/function/parse/property.hpp"
    "vkzlib/mpl/function/parse/type.hpp"
    # vkzlib/mpl/function/parse/helper/
    "vkzlib/mpl/function/parse/helper/common.hpp"
    "vkzlib/mpl/function/parse/helper/member_function_pointer.hpp"
    "vkzlib/mpl/function/parse/helper/stl_function_like.hpp"
    # vkzlib/mpl/function/parse/property/
    "vkzlib/mpl/function/parse/property/concepts.hpp"
    # vkzlib/mpl/function/parse/type/
    "vkzlib/mpl/function/parse/type/enums.hpp"
    "vkzlib/mpl/function/parse/type/concepts.hpp"
    # vkzlib/mpl/function/property/
    "vkzlib/mpl/function/property/enums.hpp"
    # vkzlib/mpl/pack/
    "vkzlib/mpl/pack/nth_of.hpp"
    "vkzlib/mpl/pack/Pack.hpp"
    # vkzlib/mpl/preprocessor/
    "vkzlib/mpl/preprocessor/and.hpp"
    "vkzlib/mpl/preprocessor/cat.hpp"
    "vkzlib/mpl/preprocessor/conditional.hpp"
    "vkzlib/mpl/preprocessor/const.hpp"
    "vkzlib/mpl/preprocessor/empty.hpp"
    "vkzlib/mpl/preprocessor/empty_tuple.hpp"
    "vkzlib/mpl/preprocessor/expand_if.hpp"
    "vkzlib/mpl/preprocessor/identity.hpp"
    "vkzlib/mpl/preprocessor/is_empty.hpp"
    "vkzlib/mpl/preprocessor/is_zero.hpp"
    "vkzlib/mpl/preprocessor/not.hpp"
    "vkzlib/mpl/preprocessor/or.hpp"
    "vkzlib/mpl/preprocessor/pick_second_or.hpp"
    "vkzlib/mpl/preprocessor/signature.hpp"
    "vkzlib/mpl/preprocessor/unpack_tuple.hpp"
    "vkzlib/mpl/preprocessor/va_opt.hpp"
    # vkzlib/mpl/preprocessor/signature/
    "vkzlib/mpl/preprocessor/signature/is_tag.hpp"
    "vkzlib/mpl/preprocessor/signature/map_syntax_product.hpp"
    "vkzlib/mpl/preprocessor/signature/tag.hpp"
    "vkzlib/mpl/preprocessor/signature/tag_to_enum.hpp"
    "vkzlib/mpl/preprocessor/signature/untag.hpp"
    # vkzlib/mpl/tpl/
    "vkzlib/mpl/tpl/fst.hpp"
    # vkzlib/mpl/tpl/fst/
    "vkzlib/mpl/tpl/fst/parse_template_spec.hpp"
    "vkzlib/mpl/tpl/fst/SameTemplateAs.hpp"
)

set_target_properties(vkzlib PROPERTIES
    INTERFACE_CXX_STANDARD 20
    INTERFACE_CXX_STANDARD_REQUIRED ON
    INTERFACE_CXX_EXTENSIONS OFF
)

if (MSVC)
    target_compile_options(vkzlib INTERFACE
        "/Zc:preprocessor"
    )
endif()

target_include_directories(vkzlib INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

if (BUILD_TESTING)
  add_subdirectory("test")
endif()

include(GNUInstallDirs)

install(
    DIRECTORY "vkzlib/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/vkzlib"
    FILES_MATCHING
    PATTERN "*.h"
    PATTERN "*.hpp"
)

install(
    TARGETS vkzlib
    EXPORT vkzlibTargets
)

install(
    EXPORT vkzlibTargets
    NAMESPACE vkzlib::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/vkzlib
)
