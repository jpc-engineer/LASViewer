cmake_minimum_required(VERSION 3.16)
project(LASViewer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

add_executable(LASViewer
    src/main.cpp
    src/glad.c
    src/core/Window.cpp
    src/graphics/Shader.cpp
    src/graphics/PointCloudMesh.cpp
    src/scene/OrbitCamera.cpp
    src/io/LASLoader.cpp
)

target_include_directories(LASViewer PRIVATE include src)

target_link_libraries(LASViewer PRIVATE
    glfw
    glm::glm
    "-framework OpenGL"
    "-framework Cocoa"
    "-framework IOKit"
)

# Copy shaders folder next to the compiled executable
add_custom_command(TARGET LASViewer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:LASViewer>/shaders
)
