
###########
# Setup
###########
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_BUILD_TYPE Debug)

if(NOT DEFINED NOVA_BUILD_EXAMPLES)
    set(NOVA_BUILD_EXAMPLES true)
endif(NOT DEFINED NOVA_BUILD_EXAMPLES)

##########
# Static Lib
##########
project(Nova)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor/harfbuzz/src)

file(GLOB_RECURSE LIB_SOURCES "src/*.cpp")
file(GLOB_RECURSE LIB_HEADERS "src/*.h")

# Add this file
list(APPEND LIB_SOURCES "cmake/build_library.cmake")

add_library(Nova STATIC ${LIB_SOURCES} ${LIB_HEADERS})

##########
# Tools
##########
file(GLOB LINT_SOURCES "src/*.cpp" "src/*.h" "test/*.cpp" "examples/*.cpp")
file(GLOB LINT_SCRIPT "tools/cpplint.py")

add_custom_target(lint
    COMMAND
    "python" ${LINT_SCRIPT} ${LINT_SOURCES}
    WORKING_DIRECTORY ./)

add_subdirectory(vendor/googletest/googletest)

enable_testing()

# Include the gtest library. gtest_SOURCE_DIR is available due to
# 'project(gtest)' above.
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

file(GLOB TEST_SOURCES "test/*.cpp")
add_executable(TestAll ${TEST_SOURCES})

# Standard linking to gtest stuff.
target_link_libraries(TestAll gtest gtest_main)

# Extra linking for the project.
find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})

target_link_libraries(TestAll Nova)
target_link_libraries(TestAll harfbuzz)
target_link_libraries(TestAll ${FREETYPE_LIBRARIES})

add_test(NovaTests TestAll)

##########
# Examples
##########
if(NOVA_BUILD_EXAMPLES)
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/build_examples.cmake)
endif(NOVA_BUILD_EXAMPLES)
