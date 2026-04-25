cmake_minimum_required(VERSION 3.10)
project(MyRenderer)

# Find the OpenMP package
find_package(OpenMP REQUIRED)

# 1. Tell CMake to include the MiniFB project
# This looks into the minifb folder and reads its configuration
add_subdirectory(minifb)

# 2. Define your executable
add_executable(my_renderer src/main.c
    src/minifb.c
    src/graphics/renderer.c
    src/graphics/mesh.c
    src/utils/math_utils.c
    src/utils/color_utils.c
    src/graphics/camera.c)

# 3. Link everything together
target_link_libraries(my_renderer PRIVATE minifb OpenMP::OpenMP_C)

# 4. Tell your code where to find the "MiniFB.h" header
target_include_directories(my_renderer PRIVATE
    include
    minifb/include
)