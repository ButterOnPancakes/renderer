# Custom Renderer

A small software renderer written in C. It loads an OBJ mesh, rasterizes triangles into a color and depth buffer, and displays the result using MiniFB.

## Requirements

- CMake 3.10 or newer
- A C compiler with OpenMP support
- The MiniFB Git submodule

If MiniFB is missing after cloning, initialize it with:

```sh
git submodule update --init --recursive
```

## Build

From the repository root, configure and compile the project:

```sh
cmake -S . -B build
cmake --build build -j
```

## Run

Run the program from the repository root so its relative path to the sample model resolves:

```sh
./build/my_renderer
```

The sample OBJ model is `assets/3.obj`. Use **Z/S** to move forward/back, **Q/D** to strafe, **Space/Left Shift** to move vertically, and **A/E** to rotate the camera. Close the window to exit.
