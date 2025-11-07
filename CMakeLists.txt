cmake_minimum_required(VERSION 3.10)
project(a)

set(CMAKE_CXX_STANDARD 17)

set(PROJECT_SOURCES
    src/main.cpp
    src/indexbuffer.cpp
    src/renderer.cpp
    src/shader.cpp
    src/vertexarray.cpp
    src/vertexbuffer.cpp
    src/shapes.cpp
)

# Add executable
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

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

#For debug mode:
# run lldb a
# These are the usable commands:
# (lldb) breakpoint set --file src/main.cpp --line 42
# (lldb) run            to start
# (lldb) next           step over
# (lldb) step          step into function
# (lldb) print myVar    show variable
# (lldb) bt             backtrace (call stack)
# (lldb) continue       to continue
# (lldb) exit           to leave