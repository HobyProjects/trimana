cmake_minimum_required(VERSION 3.6)
project(Trimana VERSION 1.0.0 LANGUAGES C CXX)

# C standards
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# C++ standards
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# cmake settings
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(BUILD_SHARED_LIBS ON)
set(CMAKE_BUILD_TYPE "Debug")

# external libraries build
add_subdirectory("lib")

# project builds
add_subdirectory("src/core")
add_subdirectory("src/engine")