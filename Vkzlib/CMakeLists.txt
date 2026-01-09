# CMakeList.txt : CMake project for VkzlibCpp, include source and define
# project specific logic here.
#

add_library(vkzlib INTERFACE)

target_include_directories(vkzlib INTERFACE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(vkzlib
	INTERFACE Boost::preprocessor
)

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
