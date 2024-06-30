cmake_minimum_required(VERSION 3.20)
project(TrimanaCorelib)

set(TRIMANA_CORE_LIBRARY TrimanaCore)
set(TRIMANA_CORE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/core)

file(GLOB TRIMANA_CORE_HEADERS_AND_SOURCES
    ${TRIMANA_CORE_DIR}/include/${TRIMANA_CORE_LIBRARY}/*.hpp
    ${TRIMANA_CORE_DIR}/src/*.cpp
)

if(NOT BUILD_SHARED_LIBS)
    add_library(${TRIMANA_CORE_LIBRARY} STATIC EXCLUDE_FROM_ALL ${STB_IMAGE_HEADER} ${TRIMANA_CORE_HEADERS_AND_SOURCES})
else()
    add_library(${TRIMANA_CORE_LIBRARY} SHARED EXCLUDE_FROM_ALL ${STB_IMAGE_HEADER} ${TRIMANA_CORE_HEADERS_AND_SOURCES})
endif()

target_include_directories(${TRIMANA_CORE_LIBRARY} PUBLIC ${STB_IMAGE_SOURCE_DIR})

target_include_directories(${TRIMANA_CORE_LIBRARY} PUBLIC
    $<BUILD_INTERFACE:${TRIMANA_CORE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_include_directories(${TRIMANA_CORE_LIBRARY} PRIVATE
    $<BUILD_INTERFACE:${TRIMANA_CORE_DIR}/include/${TRIMANA_CORE_LIBRARY}>
    $<INSTALL_INTERFACE:include/${TRIMANA_CORE_LIBRARY}>
)

target_link_libraries(
    ${TRIMANA_CORE_LIBRARY} PUBLIC 
    SDL3::SDL3
    spdlog::spdlog_header_only
    libglew_static
    glm::glm-header-only
    assimp
    ImGui::ImGui
)

add_library(TRIMANA::CORE ALIAS ${TRIMANA_CORE_LIBRARY})