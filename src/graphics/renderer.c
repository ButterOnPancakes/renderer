#include "graphics/renderer.h"

#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <omp.h>

void renderer_init(Renderer *renderer, int width, int height) {
    assert(renderer != NULL && width > 0 && height > 0);

    renderer->width = width;
    renderer->height = height;

    renderer->frame_buffer = malloc(width * height * sizeof(Color));
    renderer->depth_buffer = malloc(width * height * sizeof(float));
    assert(renderer->frame_buffer != NULL && renderer->depth_buffer != NULL);
    
    for(int i = 0; i < width * height; i++) {
        renderer->frame_buffer[i] = 0;
        renderer->depth_buffer[i] = FLT_MAX;
    }
}
void renderer_destroy(Renderer *renderer) {
    assert(renderer != NULL);
    free(renderer->frame_buffer);
    free(renderer->depth_buffer);
}

// Rendering
Vec3 project_vertex(Vertex v, Mat4 MVP, Mat4 viewport) {
    Vec4 p;
    p.x = v.x; p.y = v.y; p.z = v.z; p.w = 1;
    p = mat4_vec4_mul(MVP, p);
    p = vec4_ndc(p);
    p = mat4_vec4_mul(viewport, p);
    return (Vec3) {.x = p.x, .y = p.y, .z = p.z};
}
void renderer_draw_mesh(Renderer *renderer, Mesh *mesh, Transform trans, Camera *cam, Color color) {
    assert(renderer != NULL && mesh != NULL && mesh->faces != NULL && mesh->size > 0);

    Mat4 model = mat4_model(trans);
    Mat4 view = mat4_view(cam);
    Mat4 proj = mat4_identity();//mat4_projection(cam);
    Mat4 MVP = mat4_mul(proj, mat4_mul(view, model));
    Mat4 viewport = mat4_viewport(0, 0, renderer->width, renderer->height);

    for (size_t i = 0; i < mesh->size; i++) {
        Vec3 p1 = project_vertex(mesh->faces[i].v1, MVP, viewport);
        Vec3 p2 = project_vertex(mesh->faces[i].v2, MVP, viewport);
        Vec3 p3 = project_vertex(mesh->faces[i].v3, MVP, viewport);

        // Backface culling
        if (signed_triangle_area(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y) <= 0) continue;

        renderer_draw_filled_triangle(renderer, p1, p2, p3, mesh->faces[i].color);
    }
}

// 2D stuff
void renderer_clear(Renderer *renderer, Color color) {
    assert(renderer != NULL);
    int size = renderer->width * renderer->height;
    for (int i = 0; i < size; i++) {
        renderer->depth_buffer[i] = FLT_MAX;
    }
    memset(renderer->frame_buffer, 0, size * sizeof(Color));
}
void renderer_draw_line(Renderer *renderer, Vec3 p1, Vec3 p2, Color color) {
    int x1 = p1.x, y1 = p1.y;
    int x2 = p2.x, y2 = p2.y;
    assert(renderer != NULL && 
        0 <= x1 && x1 < renderer->width && 0 <= y1 && y1 < renderer->height &&
        0 <= x2 && x2 < renderer->width && 0 <= y2 && y2 < renderer->height
    );
    float dx = x2 - x1, dy = y2 - y1;
    int steps = abs(dx) >= abs(dy) ? abs(dx) : abs(dy); //To avoid skipping dots
    dx = dx / steps; dy = dy / steps;
    float x = x1, y = y1;

    float z = p1.z, dz = (p2.z - p1.z) / (float)steps;
    for (int i = 0; i < steps; i++) {
        renderer->depth_buffer[(int)(x + y * renderer->width)] = z;
        renderer->frame_buffer[(int)(x + y * renderer->width)] = color;
        x += dx;
        y += dy;
        z += dz;
    }
}

void renderer_draw_filled_triangle(Renderer *renderer, Vec3 p1, Vec3 p2, Vec3 p3, Color color) {
    int ax = p1.x; int ay = p1.y;
    int bx = p2.x; int by = p2.y;
    int cx = p3.x; int cy = p3.y;
    
    int min_x = min(min(ax, bx), cx); int min_y = min(min(ay, by), cy);
    int max_x = max(max(ax, bx), cx); int max_y = max(max(ay, by), cy);
    float total_area = signed_triangle_area(ax, ay, bx, by, cx, cy);
    if (fabs(total_area) <= FLT_EPSILON) return;
    float inv_total_area = 1.0 / total_area;

    #pragma omp parallel for
    for (int x = min_x; x <= max_x; x++) {
        for (int y = min_y; y <= max_y; y++) {
            if(!(0 <= x && x < renderer->width && 0 <= y && y < renderer->height)) continue;

            float alpha = signed_triangle_area(x, y, bx, by, cx, cy) * inv_total_area;
            float beta  = signed_triangle_area(x, y, cx, cy, ax, ay) * inv_total_area;
            float gamma = signed_triangle_area(x, y, ax, ay, bx, by) * inv_total_area;
            if (alpha < 0 || beta < 0 || gamma < 0) continue; // negative barycentric coordinate => the pixel is outside the triangle

            float z = alpha * p1.z + beta * p2.z + gamma * p3.z;
            if (z < renderer->depth_buffer[x + y * renderer->width]) {
                renderer->depth_buffer[x + y * renderer->width] = z;
                renderer->frame_buffer[x + y * renderer->width] = color;
            }
        }
    }
}