cmake_minimum_required(VERSION 3.28)
project(tic-tac-toe)

set(CMAKE_CXX_STANDARD 17)

enable_language(C)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Freetype REQUIRED)

include_directories(
    ${CMAKE_SOURCE_DIR}/src/engine
    ${CMAKE_SOURCE_DIR}/src/thirdparty
    ${CMAKE_SOURCE_DIR}/src/thirdparty/imgui
    ${CMAKE_SOURCE_DIR}/src/thirdparty/imgui/backends
    ${FREETYPE_INCLUDE_DIRS}
)

set(IMGUI_SOURCES
    src/thirdparty/imgui/imgui.cpp
    src/thirdparty/imgui/imgui_draw.cpp
    src/thirdparty/imgui/imgui_tables.cpp
    src/thirdparty/imgui/imgui_widgets.cpp
    src/thirdparty/imgui/backends/imgui_impl_glfw.cpp
    src/thirdparty/imgui/backends/imgui_impl_opengl3.cpp
)

set(UTIL_SOURCES
    src/engine/core/resources/shader.cpp
    src/engine/core/resources/texture.cpp
    src/engine/core/resources/sprite.cpp
    src/engine/core/primitives/rect.cpp
    src/engine/core/primitives/line.cpp
    src/engine/core/resource_manager.cpp
    src/engine/gui/gui_item.cpp
    src/engine/gui/gui_system.cpp
    src/engine/gui/text/text_renderer.cpp
    src/engine/gui/widgets/text_field.cpp
    src/engine/gui/widgets/menu.cpp
    src/engine/gui/widgets/button.cpp
    src/engine/gui/widgets/dropdown.cpp
)

set(GLAD_SOURCE src/thirdparty/glad.c)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(tic-tac-toe
    src/main.cpp
    src/game.cpp
    src/board.cpp
    src/tile.cpp
    src/player.cpp
    src/player_manager.cpp
    ${UTIL_SOURCES}
    ${IMGUI_SOURCES}
    ${GLAD_SOURCE}
)

set_source_files_properties(${GLAD_SOURCE} PROPERTIES LANGUAGE C)

target_link_libraries(tic-tac-toe
    glfw
    OpenGL::GL
    Freetype::Freetype
    dl
    pthread
)

target_include_directories(tic-tac-toe PRIVATE ${CMAKE_SOURCE_DIR}/engine)

set_target_properties(tic-tac-toe PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

# Compiler options
target_compile_options(tic-tac-toe PRIVATE
    -O3
    -march=x86-64
)
