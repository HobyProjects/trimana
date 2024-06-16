set(
    IMGUI_SOURCE_DIR 
    ${PROJECT_SOURCE_DIR}/lib/imgui
    ${PROJECT_SOURCE_DIR}/lib/imgui/backends
)

set(
    IMGUI_HEADERS
    ${PROJECT_SOURCE_DIR}/lib/imgui/imconfig.h 
    ${PROJECT_SOURCE_DIR}/lib/imgui/imgui_internal.h
    ${PROJECT_SOURCE_DIR}/lib/imgui/imgui.h
    ${PROJECT_SOURCE_DIR}/lib/imgui/imstb_rectpack.h
    ${PROJECT_SOURCE_DIR}/lib/imgui/imstb_textedit.h 
    ${PROJECT_SOURCE_DIR}/lib/imgui/imstb_truetype.h
    
    ${PROJECT_SOURCE_DIR}/lib/imgui/backends/imgui_impl_sdl3.h 
    ${PROJECT_SOURCE_DIR}/lib/imgui/backends/imgui_impl_opengl3.h 
    ${PROJECT_SOURCE_DIR}/lib/imgui/backends/imgui_impl_opengl3_loader.h 
)

set(
    IMGUI_SOURCE
    ${PROJECT_SOURCE_DIR}/lib/imgui/imgui.cpp 
    ${PROJECT_SOURCE_DIR}/lib/imgui/imgui_draw.cpp 
    ${PROJECT_SOURCE_DIR}/lib/imgui/imgui_widgets.cpp 
    ${PROJECT_SOURCE_DIR}/lib/imgui/imgui_tables.cpp 
    ${PROJECT_SOURCE_DIR}/lib/imgui/imgui_demo.cpp
    
    ${PROJECT_SOURCE_DIR}/lib/imgui/backends/imgui_impl_sdl3.cpp 
    ${PROJECT_SOURCE_DIR}/lib/imgui/backends/imgui_impl_opengl3.cpp
)

set(IMGUI_LIBRARY  "imgui")
if(NOT BUILD_SHARED_LIBS)
    message("-- IMGUI BUID STATIC : ON")
    message("-- IMGUI BUID SHARED : OFF")
    add_library(${IMGUI_LIBRARY} STATIC ${IMGUI_HEADERS} ${IMGUI_SOURCE})    
else()
    message("-- IMGUI BUID STATIC : OFF")
    message("-- IMGUI BUID SHARED : ON")
    add_library(${IMGUI_LIBRARY} SHARED ${IMGUI_HEADERS} ${IMGUI_SOURCE})
endif()

target_include_directories(${IMGUI_LIBRARY} PUBLIC ${IMGUI_SOURCE_DIR})
target_link_libraries(
    ${IMGUI_LIBRARY} PRIVATE 
    SDL3::SDL3
)

add_library(ImGui::ImGui ALIAS ${IMGUI_LIBRARY})


# ------------------------------------------------------------------------------------------------------


set(
    TRIMANA_CORE_SRC_DIR
    ${PROJECT_SOURCE_DIR}/lib
    ${PROJECT_SOURCE_DIR}/src/core
)

set(
    TRIMANA_CORE_HEADERS
    ${PROJECT_SOURCE_DIR}/src/core/entry/core.hpp
    ${PROJECT_SOURCE_DIR}/src/core/utils/logger.hpp
    ${PROJECT_SOURCE_DIR}/src/core/events/events.hpp
    ${PROJECT_SOURCE_DIR}/src/core/events/events_window.hpp
    ${PROJECT_SOURCE_DIR}/src/core/events/events_mouse.hpp
    ${PROJECT_SOURCE_DIR}/src/core/events/events_keyboard.hpp
    ${PROJECT_SOURCE_DIR}/src/core/window/sdl_window.hpp
    ${PROJECT_SOURCE_DIR}/src/core/window/window.hpp
    ${PROJECT_SOURCE_DIR}/src/core/renders/GL/gl.hpp
    ${PROJECT_SOURCE_DIR}/src/core/model/model.hpp
    ${PROJECT_SOURCE_DIR}/src/core/ecs/ecs.hpp
    ${PROJECT_SOURCE_DIR}/src/core/gui/sdl_gui.hpp
    ${PROJECT_SOURCE_DIR}/lib/stb/stb_image.h
)

set(
    TRIMANA_CORE_SOURCES
    ${PROJECT_SOURCE_DIR}/src/core/entry/core.cpp
    ${PROJECT_SOURCE_DIR}/src/core/renders/GL/gl.cpp
    ${PROJECT_SOURCE_DIR}/src/core/model/model.cpp
    ${PROJECT_SOURCE_DIR}/src/core/utils/logger.cpp
)

add_compile_definitions(TRIMANA_CORE_EXPORT)
set(TRIMANA_CORE_LIBRARY "tricore")

if(NOT BUILD_SHARED_LIBS)
    message("-- TRIMANA CORE BUILD STATIC : ON")
    message("-- TRIMANA CORE BUILD SHARED : OFF")
    add_library(${TRIMANA_CORE_LIBRARY} STATIC  ${TRIMANA_CORE_HEADERS} ${TRIMANA_CORE_SOURCES})
    target_include_directories(${TRIMANA_CORE_LIBRARY} PUBLIC  ${TRIMANA_CORE_SRC_DIR} ${IMGUI_SOURCE_DIR})
else()
    message("-- TRIMANA CORE BUILD STATIC : OFF")
    message("-- TRIMANA CORE BUILD SHARED : ON")
    add_library(${TRIMANA_CORE_LIBRARY} SHARED ${TRIMANA_CORE_HEADERS} ${TRIMANA_CORE_SOURCES})
    target_include_directories(${TRIMANA_CORE_LIBRARY} PUBLIC  ${TRIMANA_CORE_SRC_DIR} ${IMGUI_SOURCE_DIR}) 
endif()

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