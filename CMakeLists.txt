cmake_minimum_required(VERSION 3.10)
project(MyRenderer)

# 1. Tell CMake to include the MiniFB project
# This looks into the minifb folder and reads its configuration
add_subdirectory(minifb)

# 2. Define your executable
add_executable(my_renderer src/main.c
    src/minifb.c
    src/graphics/renderer.c
    src/graphics/obj_manager.c)

# 3. Link everything together
# This automatically handles Wayland, X11, and Math libraries for you
target_link_libraries(my_renderer PRIVATE minifb)

# 4. Tell your code where to find the "MiniFB.h" header
target_include_directories(my_renderer PRIVATE
    include
    minifb/include
)