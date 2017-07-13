
##########
# Project Configuration
##########
project(ChimeraOpengl3)

# Glob all source files
set(
    OPENGL3_EXAMPLE_SOURCES
    "examples/app/root.h"

    "examples/opengl3/opengl3.cpp"

    "src/adaptor/opengl3.cpp"
    "src/adaptor/opengl3.h"
)

# Add this file
list(APPEND OPENGL3_EXAMPLE_SOURCES "cmake/build_examples.cmake")
list(APPEND OPENGL3_EXAMPLE_SOURCES "cmake/examples/opengl3.cmake")

add_executable(ChimeraOpengl3 ${OPENGL3_EXAMPLE_SOURCES})

# Add png loading
target_sources(ChimeraOpengl3 PRIVATE "vendor/lodepng/lodepng.cpp")

# Add gif loading
target_sources(ChimeraOpengl3 PRIVATE "vendor/libnsgif/src/libnsgif.c")

foreach(source IN LISTS OPENGL3_EXAMPLE_SOURCES)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()

include_directories(./)
include_directories(vendor)
include_directories(vendor/libnsgif/include)
include_directories(examples)
include_directories(include)

# The version number
set(Opengl3_VERSION_MAJOR 1)
set(Opengl3_VERSION_MINOR 0)

# Include GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw/include)
target_link_libraries(ChimeraOpengl3 glfw)

# Include OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
target_link_libraries(ChimeraOpengl3 ${OPENGL_LIBRARY})

if(WIN32)
    find_package(GLEW REQUIRED)
    include_directories(${GLEW_INCLUDE_DIRS})
    target_link_libraries(ChimeraOpengl3 ${GLEW_LIBRARIES})
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor/GL/include)
endif(WIN32)

##########
# Install Deps
##########
include_directories(${FREETYPE_INCLUDE_DIRS})
target_link_libraries(ChimeraOpengl3 ${FREETYPE_LIBRARIES})

include_directories(${HARFBUZZ_INCLUDE_DIR})
target_link_libraries(ChimeraOpengl3 ${HARFBUZZ_LIBRARY})

target_link_libraries(ChimeraOpengl3 Chimera)

if (APPLE)
    target_compile_options(Chimera PRIVATE "-Werror")
    target_compile_options(Chimera PRIVATE "-Wextra")
endif (APPLE)
if (MSVC)
    target_compile_options(Chimera PRIVATE "/WX")
    target_compile_options(Chimera PRIVATE "/Wall")
endif (MSVC)

# Copy assets to build directory
file(COPY examples/assets DESTINATION ${CMAKE_BINARY_DIR})
