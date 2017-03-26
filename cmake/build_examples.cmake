
if(WIN32)
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/examples/dx9.cmake)
else()
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/examples/opengl3.cmake)
endif(WIN32)
