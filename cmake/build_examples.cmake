
##########
# Project Configuration
##########
project (Opengl3)

# Glob all source files
file(GLOB EXAMPLE_SOURCES "examples/*.cpp")
file(GLOB EXAMPLE_HEADERS "examples/*.h")

# Add png loading
list(APPEND EXAMPLE_SOURCES "vendor/lodepng/lodepng.cpp")

# Add gif loading
list(APPEND EXAMPLE_SOURCES "vendor/libnsgif/src/libnsgif.c")

# Include GLEW on Windows
if(WIN32)
    list(APPEND EXAMPLE_SOURCES "vendor/glew/src/glew.c")
endif(WIN32)

# Add this file
list(APPEND EXAMPLE_SOURCES "cmake/build_examples.cmake")

add_executable(Opengl3 ${EXAMPLE_SOURCES} ${EXAMPLE_HEADERS})

include_directories(./)
include_directories(vendor)
include_directories(vendor/libnsgif/include)
include_directories(examples)

# The version number
set(Opengl3_VERSION_MAJOR 1)
set(Opengl3_VERSION_MINOR 0)

# Include SDL2
# set(CMAKE_MACOSX_RPATH 1)
# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/sdl2)
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor/sdl2/include)
# target_link_libraries(Opengl3 SDL2-static)

# Include GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw)
target_link_libraries(Opengl3 glfw)

# Include OpenCV
# if(WIN32)
#     include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor/opencv)
#     include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor/opencv/include)
#     include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor/opencv/modules/core/include)
#     include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor/opencv/modules/calib3d/include)
#     include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor/opencv/modules/features2d/include)
#     include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor/opencv/modules/flann/include)
#     include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor/opencv/modules/highgui/include)
#     include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor/opencv/modules/imgcodecs/include)
#     include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor/opencv/modules/imgproc/include)
#     include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor/opencv/modules/ml/include)
#     include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor/opencv/modules/objdetect/include)
#     include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor/opencv/modules/photo/include)
#     include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor/opencv/modules/shape/include)
#     include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor/opencv/modules/stitching/include)
#     include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor/opencv/modules/superres/include)
#     include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor/opencv/modules/video/include)
#     include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor/opencv/modules/videoio/include)
#     include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor/opencv/modules/videostab/include)
#     target_link_libraries(Nova ${CMAKE_CURRENT_SOURCE_DIR}/vendor/opencv/lib/Debug/opencv_core310d.lib)
#     target_link_libraries(Nova ${CMAKE_CURRENT_SOURCE_DIR}/vendor/opencv/lib/Debug/opencv_video310d.lib)
#     target_link_libraries(Nova ${CMAKE_CURRENT_SOURCE_DIR}/vendor/opencv/lib/Debug/opencv_videoio310d.lib)
# else()
#     find_package(OpenCV REQUIRED)
#     include_directories(${OpenCV_INCLUDE_DIRS})
#     target_link_libraries(Opengl3 ${OpenCV_LIBS})
# endif(WIN32)

# Include GLEW and Freetype on Windows
if(WIN32)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor/glew/include)
endif(WIN32)

# Include OpenGL
find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
    include_directories(${OPENGL_INCLUDE_DIRS})
    target_link_libraries(Opengl3 ${OPENGL_LIBRARY})
endif(OPENGL_FOUND)

##########
# Install Deps
##########
find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})

# Linkage
target_link_libraries(Opengl3 Nova)
target_link_libraries(Opengl3 harfbuzz)
target_link_libraries(Opengl3 ${FREETYPE_LIBRARIES})

# Copy assets to build directory
file(COPY examples/assets DESTINATION ${CMAKE_BINARY_DIR})
