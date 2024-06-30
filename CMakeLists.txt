cmake_minimum_required(VERSION 3.24)
project(Trimana)

# C standards
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# C++ standards
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# cmake settings
#list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_PDB_OUTPUT_DIRECTORY     ${CMAKE_BINARY_DIR})

set(CMAKE_BUILD_TYPE Debug)
set(BUILD_SHARED_LIBS OFF)

# external libraries build
add_subdirectory(lib)

# project builds

file(GLOB RESOURCES_FILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/res/shaders/*.glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/res/shaders/*.glsl
)

set(STB_IMAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/stb)
set(STB_IMAGE_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/lib/stb/stb_image.h)

add_subdirectory(src/core)
add_subdirectory(src/engine)