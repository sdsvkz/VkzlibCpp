# CMakeList.txt : CMake project for VkzlibCpp, include source and define
# project specific logic here.
#

if(VKZLIB_USE_MODULES)

else()

  add_library(vkzlib INTERFACE
      # vkzlib/
      "include/vkzlib/mpl.hpp"
      # vkzlib/mpl/
      "include/vkzlib/mpl/common.hpp"
      "include/vkzlib/mpl/function.hpp"
      "include/vkzlib/mpl/pack.hpp"
      "include/vkzlib/mpl/preprocessor.hpp"
      "include/vkzlib/mpl/tpl.hpp"
      # vkzlib/mpl/common/
      "include/vkzlib/mpl/common/AnyOf.hpp"
      "include/vkzlib/mpl/common/Class.hpp"
      "include/vkzlib/mpl/common/conditionalCall.hpp"
      "include/vkzlib/mpl/common/CString.hpp"
      "include/vkzlib/mpl/common/dup_const.hpp"
      "include/vkzlib/mpl/common/dup_cvref.hpp"
      "include/vkzlib/mpl/common/dup_reference.hpp"
      "include/vkzlib/mpl/common/dup_volatile.hpp"
      "include/vkzlib/mpl/common/HasCommonType.hpp"
      "include/vkzlib/mpl/common/Homogeneous.hpp"
      # vkzlib/mpl/common/internal/
      "include/vkzlib/mpl/common/internal/NPOS.hpp"
      # vkzlib/mpl/common/ce/
      "include/vkzlib/mpl/common/ce/findFirstFor.hpp"
      "include/vkzlib/mpl/common/ce/min.hpp"
      # vkzlib/mpl/function/
      "include/vkzlib/mpl/function/Fn.hpp"
      "include/vkzlib/mpl/function/FnRef.hpp"
      "include/vkzlib/mpl/function/MonomorphicFunctor.hpp"
      "include/vkzlib/mpl/function/parse.hpp"
      "include/vkzlib/mpl/function/property.hpp"
      "include/vkzlib/mpl/function/SameArgsAs.hpp"
      "include/vkzlib/mpl/function/SameResultAs.hpp"
      # vkzlib/mpl/function/internal/
      "include/vkzlib/mpl/function/internal/DefaultPack.hpp"
      # vkzlib/mpl/function/parse/
      "include/vkzlib/mpl/function/parse/helper.hpp"
      "include/vkzlib/mpl/function/parse/parse.hpp"
      "include/vkzlib/mpl/function/parse/property.hpp"
      "include/vkzlib/mpl/function/parse/type.hpp"
      # vkzlib/mpl/function/parse/helper/
      "include/vkzlib/mpl/function/parse/helper/common.hpp"
      "include/vkzlib/mpl/function/parse/helper/member_function_pointer.hpp"
      "include/vkzlib/mpl/function/parse/helper/stl_function_like.hpp"
      # vkzlib/mpl/function/parse/property/
      "include/vkzlib/mpl/function/parse/property/concepts.hpp"
      # vkzlib/mpl/function/parse/type/
      "include/vkzlib/mpl/function/parse/type/enums.hpp"
      "include/vkzlib/mpl/function/parse/type/concepts.hpp"
      # vkzlib/mpl/function/property/
      "include/vkzlib/mpl/function/property/enums.hpp"
      # vkzlib/mpl/pack/
      "include/vkzlib/mpl/pack/nth_of.hpp"
      "include/vkzlib/mpl/pack/Pack.hpp"
      # vkzlib/mpl/preprocessor/
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
      # vkzlib/mpl/preprocessor/signature/
      "include/vkzlib/mpl/preprocessor/signature/is_tag.hpp"
      "include/vkzlib/mpl/preprocessor/signature/map_syntax_product.hpp"
      "include/vkzlib/mpl/preprocessor/signature/tag.hpp"
      "include/vkzlib/mpl/preprocessor/signature/tag_to_enum.hpp"
      "include/vkzlib/mpl/preprocessor/signature/untag.hpp"
      # vkzlib/mpl/tpl/
      "include/vkzlib/mpl/tpl/fst.hpp"
      # vkzlib/mpl/tpl/fst/
      "include/vkzlib/mpl/tpl/fst/parse_template_spec.hpp"
      "include/vkzlib/mpl/tpl/fst/SameTemplateAs.hpp"
      "include/vkzlib/mpl/tpl/fst/WithTemplateParams.hpp"
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
  endif ()

  target_include_directories(vkzlib INTERFACE
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  )

  if (BUILD_TESTING)
    add_subdirectory("test")
  endif ()

endif()

include(GNUInstallDirs)

if(NOT VKZLIB_USE_MODULES)
install(
    DIRECTORY "include/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    FILES_MATCHING
    PATTERN "*.h"
    PATTERN "*.hpp"
)
endif()

install(
    TARGETS vkzlib
    EXPORT vkzlibTargets
)

install(
    EXPORT vkzlibTargets
    NAMESPACE vkzlib::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/vkzlib
)
