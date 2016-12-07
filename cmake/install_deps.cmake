
if(WIN32)
    execute_process(COMMAND conan install ${CMAKE_SOURCE_DIR}/cmake/win32)
else()
    execute_process(COMMAND conan install ${CMAKE_SOURCE_DIR}/cmake/unix)
endif(WIN32)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()