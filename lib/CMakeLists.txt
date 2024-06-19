cmake_minimum_required(VERSION 3.20)
project(TrimanaLibs VERSION 1.0.0 LANGUAGES C CXX)

# SDL Configuration
set(SDL_SHARED OFF)
set(SDL_STATIC ON)
add_subdirectory("SDL" EXCLUDE_FROM_ALL)

# spdlog Configuration
add_subdirectory("spdlog" EXCLUDE_FROM_ALL)

# glm lib Configuration
set(GLM_BUILD_TESTS ON)
add_subdirectory("glm" EXCLUDE_FROM_ALL)

# glew lib Configuration
set(glew-cmake_BUILD_STATIC ON)
set(glew-cmake_BUILD_SHARED OFF)
set(ONLY_LIBS ON)
add_subdirectory("glew-cmake" EXCLUDE_FROM_ALL)

# assimp lib configuration
add_subdirectory("assimp" EXCLUDE_FROM_ALL)


# imgui configuration
set(IMGUI_DIR ${PROJECT_SOURCE_DIR}/imgui)
set(IMGUI_BACKENDS_DIR ${PROJECT_SOURCE_DIR}/imgui/backends)
set(IMGUI_LIBRARY imgui)

set(
    IMGUI_HEADERS
    ${IMGUI_DIR}/imconfig.h 
    ${IMGUI_DIR}/imgui_internal.h
    ${IMGUI_DIR}/imgui.h
    ${IMGUI_DIR}/imstb_rectpack.h
    ${IMGUI_DIR}/imstb_textedit.h 
    ${IMGUI_DIR}/imstb_truetype.h
    ${IMGUI_BACKENDS_DIR}/imgui_impl_sdl3.h 
    ${IMGUI_BACKENDS_DIR}/imgui_impl_opengl3.h 
    ${IMGUI_BACKENDS_DIR}/imgui_impl_opengl3_loader.h 
)

set(
    IMGUI_SOURCE
    ${IMGUI_DIR}/imgui.cpp 
    ${IMGUI_DIR}/imgui_draw.cpp 
    ${IMGUI_DIR}/imgui_widgets.cpp 
    ${IMGUI_DIR}/imgui_tables.cpp 
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_BACKENDS_DIR}/imgui_impl_sdl3.cpp 
    ${IMGUI_BACKENDS_DIR}/imgui_impl_opengl3.cpp
)


if(NOT BUILD_SHARED_LIBS)
    add_library(${IMGUI_LIBRARY} STATIC EXCLUDE_FROM_ALL)
    target_sources(${IMGUI_LIBRARY} PUBLIC  ${IMGUI_HEADERS} PRIVATE ${IMGUI_SOURCE})
    target_include_directories(${IMGUI_LIBRARY} PUBLIC ${IMGUI_DIR} ${IMGUI_BACKENDS_DIR})    
else()
    add_library(${IMGUI_LIBRARY} SHARED EXCLUDE_FROM_ALL)
    target_sources(${IMGUI_LIBRARY} PUBLIC  ${IMGUI_HEADERS} PRIVATE ${IMGUI_SOURCE})
    target_include_directories(${IMGUI_LIBRARY} PUBLIC ${IMGUI_DIR} ${IMGUI_BACKENDS_DIR}) 
endif()

target_include_directories(${IMGUI_LIBRARY} INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/imgui>
  $<INSTALL_INTERFACE:imgui>
)

target_link_libraries(
    ${IMGUI_LIBRARY} PRIVATE 
    SDL3::SDL3
)

add_library(ImGui::ImGui ALIAS ${IMGUI_LIBRARY})

