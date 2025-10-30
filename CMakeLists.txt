cmake_minimum_required(VERSION 3.10)
project(a)

set(CMAKE_CXX_STANDARD 17)

# Add executable
add_executable(${PROJECT_NAME} src/main.cpp src/indexbuffer.cpp src/renderer.cpp src/shader.cpp src/vertexarray.cpp src/vertexbuffer.cpp src/shapes.cpp)

add_subdirectory(dependancies/glm)

target_link_libraries(${PROJECT_NAME} glm::glm)

# --- GLFW ---
add_subdirectory(dependancies/glfw)

target_link_libraries(${PROJECT_NAME} glfw)

# --- GLEW ---
add_library(glew STATIC
    dependancies/glew/src/glew.c
)

target_include_directories(glew PUBLIC
    dependancies/glew/include
)

target_link_libraries(${PROJECT_NAME} glew)

# --- macOS OpenGL framework ---
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})

# (macOS frameworks required for windowing)
target_link_libraries(${PROJECT_NAME}
    "-framework Cocoa"
    "-framework IOKit"
    "-framework CoreVideo"
)
