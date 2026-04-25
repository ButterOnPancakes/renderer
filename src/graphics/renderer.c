#include "graphics/renderer.h"

#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdbool.h>
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
typedef struct {
    Vec3 pos;
    bool visible;
} ProjectedVertex;

ProjectedVertex project_vertex(Vertex v, Mat4 MVP, Mat4 viewport) {
    Vec4 p;
    p.x = v.x; p.y = v.y; p.z = v.z; p.w = 1;
    p = mat4_vec4_mul(MVP, p);
    
    // Near plane clipping check
    if (p.w <= 0) {
        return (ProjectedVertex){.visible = false};
    }

    p = vec4_ndc(p);
    p = mat4_vec4_mul(viewport, p);
    return (ProjectedVertex){.pos = (Vec3) {.x = p.x, .y = p.y, .z = p.z}, .visible = true};
}
void renderer_draw_mesh(Renderer *renderer, Mesh *mesh, Transform trans, Camera *cam, Color color) {
    assert(renderer != NULL && mesh != NULL && mesh->faces != NULL && mesh->num_faces > 0);

    Mat4 model = mat4_model(trans);
    Mat4 view = mat4_view(cam);
    Mat4 proj = mat4_projection(cam);
    Mat4 MVP = mat4_mul(proj, mat4_mul(view, model));
    Mat4 viewport = mat4_viewport(0, 0, renderer->width, renderer->height);

    // 1. Create a cache for projected vertices
    ProjectedVertex *vertex_cache = malloc(mesh->num_vertices * sizeof(ProjectedVertex));
    assert(vertex_cache != NULL);

    // 2. Project all vertices once
    #pragma omp parallel for
    for (size_t i = 0; i < mesh->num_vertices; i++) {
        vertex_cache[i] = project_vertex(mesh->vertices[i], MVP, viewport);
    }

    // 3. Draw faces using cached vertices
    for (size_t i = 0; i < mesh->num_faces; i++) {
        ProjectedVertex pv1 = vertex_cache[mesh->faces[i].v1];
        ProjectedVertex pv2 = vertex_cache[mesh->faces[i].v2];
        ProjectedVertex pv3 = vertex_cache[mesh->faces[i].v3];

        if (!pv1.visible || !pv2.visible || !pv3.visible) continue;

        Vec3 p1 = pv1.pos;
        Vec3 p2 = pv2.pos;
        Vec3 p3 = pv3.pos;

        // Backface culling: After viewport flip, CCW (front) becomes negative area.
        // We cull triangles with area >= 0 (backfaces or degenerate).
        if (signed_triangle_area(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y) >= 0) continue;

        renderer_draw_filled_triangle(renderer, p1, p2, p3, mesh->faces[i].color);
    }

    free(vertex_cache);
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
    int steps = fabsf(dx) >= fabsf(dy) ? (int)fabsf(dx) : (int)fabsf(dy); //To avoid skipping dots
    dx = dx / (float)steps; dy = dy / (float)steps;
    float x = x1, y = y1;

    float z = p1.z, dz = (p2.z - p1.z) / (float)steps;
    for (int i = 0; i < steps; i++) {
        int idx = (int)(x + y * renderer->width);
        renderer->depth_buffer[idx] = z;
        renderer->frame_buffer[idx] = color;
        x += dx;
        y += dy;
        z += dz;
    }
}

void renderer_draw_filled_triangle(Renderer *renderer, Vec3 p1, Vec3 p2, Vec3 p3, Color color) {
    float ax = p1.x; float ay = p1.y;
    float bx = p2.x; float by = p2.y;
    float cx = p3.x; float cy = p3.y;
    
    int min_x = (int)floorf(min(min(ax, bx), cx)); 
    int min_y = (int)floorf(min(min(ay, by), cy));
    int max_x = (int)ceilf(max(max(ax, bx), cx)); 
    int max_y = (int)ceilf(max(max(ay, by), cy));

    float total_area = signed_triangle_area(ax, ay, bx, by, cx, cy);
    if (fabsf(total_area) <= FLT_EPSILON) return;
    float inv_total_area = 1.0f / total_area;

    #pragma omp parallel for
    for (int y = min_y; y <= max_y; y++) {
        for (int x = min_x; x <= max_x; x++) {
            if(!(0 <= x && x < renderer->width && 0 <= y && y < renderer->height)) continue;

            float fx = (float)x + 0.5f;
            float fy = (float)y + 0.5f;

            float alpha = signed_triangle_area(fx, fy, bx, by, cx, cy) * inv_total_area;
            float beta  = signed_triangle_area(fx, fy, cx, cy, ax, ay) * inv_total_area;
            float gamma = signed_triangle_area(fx, fy, ax, ay, bx, by) * inv_total_area;
            
            if (alpha < 0 || beta < 0 || gamma < 0) continue; 

            float z = alpha * p1.z + beta * p2.z + gamma * p3.z;
            int pixel_index = x + y * renderer->width;
            
            // Note: Simple depth test without atomic is not perfectly thread-safe but often works for simple renderers.
            // For true correctness under OpenMP, we'd need a more complex solution or critical section.
            if (z < renderer->depth_buffer[pixel_index]) {
                #pragma omp critical
                {
                    if (z < renderer->depth_buffer[pixel_index]) {
                        renderer->depth_buffer[pixel_index] = z;
                        renderer->frame_buffer[pixel_index] = color;
                    }
                }
            }
        }
    }
}
