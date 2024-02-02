cmake_minimum_required(VERSION 3.26.3)
project("OWDT-OPENGL")

find_package(PkgConfig)
pkg_search_module(GLFW REQUIRED glfw3)
find_package(OpenGL REQUIRED)
include_directories("./include/")

add_executable(run "src/glad.c" "src/window.cpp")
target_link_libraries(run ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES} "-ldl")

