
##########
# Project Configuration
##########
project(ChimeraDX9)

# Glob all source files
set(
    DX9_EXAMPLE_SOURCES
    "examples/app/root.h"

    "examples/dx9/dx9.cpp"

    "src/adaptor/dx9.cpp"
    "src/adaptor/dx9.h"
)

# Add this file
list(APPEND DX9_EXAMPLE_SOURCES "cmake/build_examples.cmake")
list(APPEND DX9_EXAMPLE_SOURCES "cmake/examples/dx9.cmake")

add_executable(CosmonautDX9 ${DX9_EXAMPLE_SOURCES})

# Add png loading
target_sources(CosmonautDX9 PRIVATE "vendor/lodepng/lodepng.cpp")

# Add gif loading
# target_sources(CosmonautDX9 PRIVATE "vendor/libnsgif/src/libnsgif.c")

foreach(source IN LISTS DX9_EXAMPLE_SOURCES)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()

set_target_properties(CosmonautDX9 PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")

include_directories(./)
include_directories(vendor)
include_directories(vendor/libnsgif/include)
include_directories(examples)
include_directories(include)

##########
# Install Deps
##########
include_directories(${FREETYPE_INCLUDE_DIRS})
target_link_libraries(CosmonautDX9 ${FREETYPE_LIBRARIES})

include_directories(${HARFBUZZ_INCLUDE_DIR})
target_link_libraries(CosmonautDX9 ${HARFBUZZ_LIBRARY})

target_link_libraries(CosmonautDX9 Chimera)

target_compile_options(CosmonautDX9 PRIVATE "/WX")
target_compile_options(CosmonautDX9 PRIVATE "/W4")

# Copy assets to build directory
file(COPY examples/assets DESTINATION ${CMAKE_BINARY_DIR})
