include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable("LibTest"
    "main.cpp"
    "vkzlib/mpl/common.cpp"
    "vkzlib/mpl/function.cpp"
    "vkzlib/mpl/pack.cpp"
    "vkzlib/mpl/tpl/fst.cpp"
)

set_target_properties("LibTest"
    PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

target_include_directories("LibTest" PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries("LibTest"
    GTest::gtest_main
    vkzlib
)

include(GoogleTest)
gtest_discover_tests(LibTest)
