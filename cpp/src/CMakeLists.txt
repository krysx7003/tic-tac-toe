add_subdirectory(board-game-engine)

include_directories(
    ${CMAKE_SOURCE_DIR}/thirdparty
    ${CMAKE_SOURCE_DIR}/thirdparty/imgui
    ${CMAKE_SOURCE_DIR}/thirdparty/imgui/backends
    ${FREETYPE_INCLUDE_DIRS}
)

set(IMGUI_SOURCES
    thirdparty/imgui/imgui.cpp
    thirdparty/imgui/imgui_draw.cpp
    thirdparty/imgui/imgui_tables.cpp
    thirdparty/imgui/imgui_widgets.cpp
    thirdparty/imgui/backends/imgui_impl_glfw.cpp
    thirdparty/imgui/backends/imgui_impl_opengl3.cpp
)

set(GLAD_SOURCE thirdparty/glad.c)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(tic-tac-toe
    main.cpp
    game.cpp
    board.cpp
    tile.cpp
    player.cpp
    player_manager.cpp
    mouse_handler.cpp
    ${IMGUI_SOURCES}
    ${GLAD_SOURCE}
)

set_source_files_properties(${GLAD_SOURCE} PROPERTIES LANGUAGE C)

target_link_libraries(tic-tac-toe
    engine
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
