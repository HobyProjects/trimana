set(TRIMANA_CORE_LIBRARY trimana_core)

set(STB_IMAGE_SOURCE_DIR ${PROJECT_SOURCE_DIR}/lib/stb)
set(STB_IMAGE_HEADER ${PROJECT_SOURCE_DIR}/lib/stb/stb_image.h)

set(TRIMANA_CORE_HEADER_DIR ${PROJECT_SOURCE_DIR}/src/core/include/${TRIMANA_CORE_LIBRARY})
set(TRIMANA_CORE_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src/core/src)

set(
    TRIMANA_CORE_HEADERS
    ${TRIMANA_CORE_HEADER_DIR}/core.hpp
    ${TRIMANA_CORE_HEADER_DIR}/logger.hpp
    ${TRIMANA_CORE_HEADER_DIR}/events.hpp
    ${TRIMANA_CORE_HEADER_DIR}/events_window.hpp
    ${TRIMANA_CORE_HEADER_DIR}/events_mouse.hpp
    ${TRIMANA_CORE_HEADER_DIR}/events_keyboard.hpp
    ${TRIMANA_CORE_HEADER_DIR}/sdl_window.hpp
    ${TRIMANA_CORE_HEADER_DIR}/window.hpp
    ${TRIMANA_CORE_HEADER_DIR}/gl.hpp
    ${TRIMANA_CORE_HEADER_DIR}/model.hpp
    ${TRIMANA_CORE_HEADER_DIR}/ecs.hpp
    ${TRIMANA_CORE_HEADER_DIR}/sdl_gui.hpp
)

set(
    TRIMANA_CORE_SOURCES
    ${TRIMANA_CORE_SOURCE_DIR}/core.cpp
    ${TRIMANA_CORE_SOURCE_DIR}/gl.cpp
    ${TRIMANA_CORE_SOURCE_DIR}/model.cpp
    ${TRIMANA_CORE_SOURCE_DIR}/logger.cpp
)

if(NOT BUILD_SHARED_LIBS)
    add_library(${TRIMANA_CORE_LIBRARY} STATIC ${STB_IMAGE_HEADER} ${TRIMANA_CORE_HEADERS} ${TRIMANA_CORE_SOURCES})
else()
    add_library(${TRIMANA_CORE_LIBRARY} SHARED ${STB_IMAGE_HEADER} ${TRIMANA_CORE_HEADERS} ${TRIMANA_CORE_SOURCES})
endif()

target_include_directories(${TRIMANA_CORE_LIBRARY} PUBLIC ${TRIMANA_CORE_HEADER_DIR} ${TRIMANA_CORE_SOURCES} ${STB_IMAGE_SOURCE_DIR})

target_include_directories(${TRIMANA_CORE_LIBRARY} PRIVATE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${TRIMANA_CORE_LIBRARY}>
    $<INSTALL_INTERFACE:include/${TRIMANA_CORE_LIBRARY}>
)

target_include_directories(${TRIMANA_CORE_LIBRARY} PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
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