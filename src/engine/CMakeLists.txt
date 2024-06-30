cmake_minimum_required(VERSION 3.20)
project(TrimanaEngine)

set(TRIMANA_ENGINE TrimanaEngine)
set(TRIMANA_ENGINE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/egnine)

file(
    GLOB TRIMANA_ENGINE_HEADERS_N_SOURCES
    ${TRIMANA_ENGINE_DIR}/include/${TRIMANA_ENGINE}/*.hpp
    ${TRIMANA_ENGINE_DIR}/src/*.cpp
)

add_executable(${TRIMANA_ENGINE} ${TRIMANA_ENGINE_HEADERS_N_SOURCES} ${RESOURCES_FILES})
target_link_libraries(${TRIMANA_ENGINE} PUBLIC TRIMANA::CORE)