set(
    TRIMANA_CORE_SRC_DIR
    ${PROJECT_SOURCE_DIR}/lib
    ${PROJECT_SOURCE_DIR}/src/core
)

set(
    TRIMANA_CORE_HEADERS
    ${PROJECT_SOURCE_DIR}/src/core/entry/init_core.hpp
    ${PROJECT_SOURCE_DIR}/src/core/utils/logger.hpp
    ${PROJECT_SOURCE_DIR}/src/core/events/events.hpp
    ${PROJECT_SOURCE_DIR}/src/core/events/events_window.hpp
    ${PROJECT_SOURCE_DIR}/src/core/events/events_mouse.hpp
    ${PROJECT_SOURCE_DIR}/src/core/events/events_keyboard.hpp
    ${PROJECT_SOURCE_DIR}/src/core/window/window.hpp
    ${PROJECT_SOURCE_DIR}/src/core/renders/GL/gl.hpp
    ${PROJECT_SOURCE_DIR}/src/core/model/model.hpp

    ${PROJECT_SOURCE_DIR}/lib/stb/stb_image.h
)

set(
    TRIMANA_CORE_SOURCES
    ${PROJECT_SOURCE_DIR}/src/core/entry/init_core.cpp
    ${PROJECT_SOURCE_DIR}/src/core/window/window.cpp
    ${PROJECT_SOURCE_DIR}/src/core/renders/GL/gl.cpp
    ${PROJECT_SOURCE_DIR}/src/core/model/model.cpp
)

set(TRIMANA_CORE_LIBRARY "trimana-core")
add_library(${TRIMANA_CORE_LIBRARY} STATIC ${TRIMANA_CORE_HEADERS} ${TRIMANA_CORE_SOURCES})
target_include_directories(${TRIMANA_CORE_LIBRARY} PUBLIC  ${TRIMANA_CORE_SRC_DIR})

target_link_libraries(
    ${TRIMANA_CORE_LIBRARY} PUBLIC 
    SDL3::SDL3
    spdlog::spdlog_header_only
    libglew_static
    glm::glm-header-only
    assimp
)