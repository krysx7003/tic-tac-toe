file(GLOB_RECURSE ENGINE_SOURCES 
    "*.cpp"
    "*.h"
)

add_library(engine STATIC ${ENGINE_SOURCES})

target_link_libraries(engine
    glfw
    OpenGL::GL
    Freetype::Freetype
)

target_include_directories(engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty
    ${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/imgui
    ${FREETYPE_INCLUDE_DIRS}
)

target_compile_options(engine PRIVATE
    -O3
    -march=x86-64
)
