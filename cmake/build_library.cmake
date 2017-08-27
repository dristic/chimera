
###########
# Setup
###########
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_BUILD_TYPE Debug)

# Options
option(CHIMERA_BUILD_EXAMPLES "Build the examples folder" OFF)

##########
# Static Lib
##########
project(Chimera)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(
    LIB_SOURCES
    "src/animation.cpp"
    "src/animation.h"
    "src/context.cpp"
    "src/context.h"
    "src/document.cpp"
    "src/document.h"
    "src/element.cpp"
    "src/element.h"
    "src/focus.cpp"
    "src/focus.h"
    "src/layout.cpp"
    "src/layout.h"
    "src/rendering.cpp"
    "src/rendering.h"

    "src/dom/body.cpp"
    "src/dom/body.h"
    "src/dom/button.cpp"
    "src/dom/button.h"
    "src/dom/div.cpp"
    "src/dom/div.h"
    "src/dom/input.cpp"
    "src/dom/input.h"

    "src/render/font.cpp"
    "src/render/font.h"

    "src/style/style.cpp"
    "src/style/style.h"

    "src/virtual/vtree.cpp"
    "src/virtual/vtree.h"

    "src/adaptor/adaptor.h"
)

# Add this file
list(APPEND LIB_SOURCES "cmake/build_library.cmake")

# find freetype
find_package(Freetype REQUIRED)
find_package(HarfBuzz REQUIRED)

include_directories(${FREETYPE_INCLUDE_DIRS})
include_directories(${HARFBUZZ_INCLUDE_DIR})

add_library(Chimera STATIC ${LIB_SOURCES})

foreach(source IN LISTS LIB_SOURCES)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()

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

target_link_libraries(TestAll Chimera)

target_link_libraries(TestAll ${FREETYPE_LIBRARIES})
target_link_libraries(TestAll ${HARFBUZZ_LIBRARY})

add_test(ChimeraTest TestAll)

if (APPLE)
    target_compile_options(Chimera PRIVATE "-Werror")
    target_compile_options(Chimera PRIVATE "-Wextra")
endif (APPLE)
if (MSVC)
    target_compile_options(Chimera PRIVATE "/WX")
    target_compile_options(Chimera PRIVATE "/W4")
endif (MSVC)

##########
# Examples
##########
if(CHIMERA_BUILD_EXAMPLES)
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/build_examples.cmake)
endif(CHIMERA_BUILD_EXAMPLES)
