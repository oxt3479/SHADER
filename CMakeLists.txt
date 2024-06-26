cmake_minimum_required(VERSION 3.19.4)
project("OWDT-OPENGL")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(PkgConfig)
#pkg_search_module(GLFW REQUIRED glfw3)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)
include_directories("./include/")

#file(GLOB SHADER_FILES "${CMAKE_SOURCE_DIR}/programs/*")
#file(COPY ${SHADER_FILES} DESTINATION "${CMAKE_BINARY_DIR}/programs")

set(SHADER_DIR "${CMAKE_SOURCE_DIR}/shaders")
set(TEXTURE_DIR "${CMAKE_SOURCE_DIR}/textures")
configure_file(config.h.in config.h)
include_directories(${CMAKE_BINARY_DIR})

message(STATUS "GLFW_LIBRARIES: ${GLFW_LIBRARIES}")
message(STATUS "OPENGL_LIBRARIES: ${OPENGL_LIBRARIES}")
message(STATUS "ASSIMP_LIBRARIES: ${ASSIMP_LIBRARIES}")
message(STATUS "GLM_LIBRARIES: ${GLM_LIBRARIES}")

file(GLOB SOURCES "src/*.cpp")
add_executable(run "src/glad.c" ${SOURCES})
set(GLFW_LIBRARIES "/usr/local/Cellar/glfw/3.4/lib/libglfw.dylib")
target_link_libraries(run ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES} glm::glm ${ASSIMP_LIBRARIES} "-ldl")