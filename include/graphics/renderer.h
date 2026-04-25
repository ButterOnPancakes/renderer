#ifndef RENDERER_H
#define RENDERER_H

#include "graphics/mesh.h"
#include "graphics/camera.h"

#include "transform.h"

#include "utils/math_utils.h"
#include "utils/color_utils.h"

typedef struct {
    int width, height;
    Color *frame_buffer;
    float *depth_buffer;
} Renderer;

void renderer_init(Renderer *renderer, int width, int height);
void renderer_destroy(Renderer *renderer);

// Actual rendering
void renderer_draw_mesh(Renderer *renderer, Mesh *mesh, Transform transform, Camera *cam, Color color);

// 2D stuff (Vec3 : (x, y, depth))
void renderer_clear(Renderer *renderer, Color color);
void renderer_draw_line(Renderer *renderer, Vec3 p1, Vec3 p2, Color color);
void renderer_draw_filled_triangle(Renderer *renderer, Vec3 p1, Vec3 p2, Vec3 p3, Color color);

#endif