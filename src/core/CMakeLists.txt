set(
    TRIMANA_CORE_SRC_DIR
    ${PROJECT_SOURCE_DIR}/src/core
)

set(
    TRIMANA_CORE_HEADERS
    ${PROJECT_SOURCE_DIR}/src/core/utils/logger.hpp
    ${PROJECT_SOURCE_DIR}/src/core/events/events.hpp
    ${PROJECT_SOURCE_DIR}/src/core/events/events_window.hpp
    ${PROJECT_SOURCE_DIR}/src/core/events/events_mouse.hpp
    ${PROJECT_SOURCE_DIR}/src/core/events/events_keyboard.hpp
    ${PROJECT_SOURCE_DIR}/src/core/window/window.hpp
)

set(
    TRIMANA_CORE_SOURCES
    ${PROJECT_SOURCE_DIR}/src/core/window/window.cpp
)

set(TRIMANA_CORE_LIBRARY "trimana-core")
add_library(${TRIMANA_CORE_LIBRARY} STATIC ${TRIMANA_CORE_HEADERS} ${TRIMANA_CORE_SOURCES})
target_include_directories(${TRIMANA_CORE_LIBRARY} BEFORE PUBLIC ${TRIMANA_CORE_SRC_DIR})

target_link_libraries(
    ${TRIMANA_CORE_LIBRARY} PUBLIC 
    SDL3::SDL3
    spdlog::spdlog_header_only
)