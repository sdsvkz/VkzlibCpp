# CMakeList.txt : CMake project for VkzlibCpp, include source and define
# project specific logic here.
#

add_library(vkzlib INTERFACE
    "vkzlib/internal/begin_shareddefs.h"
    "vkzlib/internal/end_shareddefs.h"
    "vkzlib/mpl/preprocessor.hpp"
    "vkzlib/mpl/core.hpp"
    "vkzlib/mpl/function.hpp"
    "vkzlib/mpl.hpp"
)

set_target_properties(vkzlib PROPERTIES
    INTERFACE_CXX_STANDARD 20
    INTERFACE_CXX_STANDARD_REQUIRED ON
    INTERFACE_CXX_EXTENSIONS OFF
)

if (MSVC)
    target_compile_options(vkzlib INTERFACE
        "/Zc:preprocessor"
        "/Zc:__cplusplus"
        "/permissive-"
    )
endif()

target_include_directories(vkzlib INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

include(CTest)

if (BUILD_TESTING)

  add_subdirectory("test")

endif()

install(
    TARGETS vkzlib
    EXPORT vkzlibTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
