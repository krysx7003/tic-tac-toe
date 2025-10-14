cmake_minimum_required(VERSION 3.28)
project(tic-tac-toe)

set(CMAKE_CXX_STANDARD 17)
enable_language(C)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Freetype REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src)
