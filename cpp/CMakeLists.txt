cmake_minimum_required(VERSION 3.28)
project(tic-tac-toe)

set(CMAKE_CXX_STANDARD 17)

enable_language(C)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(
    src/include
    src/include/imgui
    src/include/imgui/backends
)

set(IMGUI_SOURCES
    src/include/imgui/imgui.cpp
    src/include/imgui/imgui_draw.cpp
    src/include/imgui/imgui_tables.cpp
    src/include/imgui/imgui_widgets.cpp
    src/include/imgui/backends/imgui_impl_glfw.cpp
    src/include/imgui/backends/imgui_impl_opengl3.cpp
)

set(UTIL_SOURCES
    src/include/utils/shader.cpp
    src/include/utils/texture.cpp
    src/include/utils/resource_manager.cpp
    src/include/utils/sprite.cpp
)

set(GLAD_SOURCE src/include/glad.c)

add_executable(tic-tac-toe
    src/main.cpp
    src/game.cpp
    src/board.cpp
    ${UTIL_SOURCES}
    ${IMGUI_SOURCES}
    ${GLAD_SOURCE}
)

set_source_files_properties(${GLAD_SOURCE} PROPERTIES LANGUAGE C)

target_link_libraries(tic-tac-toe
    glfw
    OpenGL::GL
    dl
    pthread
)

set_target_properties(tic-tac-toe PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

# Compiler options
target_compile_options(tic-tac-toe PRIVATE
    -O3
    -march=x86-64
)
