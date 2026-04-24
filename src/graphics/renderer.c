#include "renderer.h"

#include "utils/math_utils.h"

#include <assert.h>
#include <stdlib.h>
#include <omp.h>

void buffer_init(FrameBuffer *buffer, int width, int height) {
    assert(buffer != NULL && width > 0 && height > 0);

    buffer->width = width;
    buffer->height = height;

    buffer->buffer = calloc(width * height, sizeof(Color));
}
void buffer_destroy(FrameBuffer *buffer) {
    assert(buffer != NULL);
    free(buffer->buffer);
}

void buffer_clear(FrameBuffer *buffer, Color color) {
    assert(buffer != NULL);
    for (int i = 0; i < buffer->width * buffer->height; i++) buffer->buffer[i] = color;
}

void buffer_draw_pixel(FrameBuffer *buffer, int x, int y, Color color) {
    assert(buffer != NULL && 0 <= x && x < buffer->width && 0 <= y && y < buffer->height);
    buffer->buffer[x + y * buffer->width] = color;
}
void buffer_draw_line(FrameBuffer *buffer, int x1, int y1, int x2, int y2, Color color) {
    assert(buffer != NULL && 
        0 <= x1 && x1 < buffer->width && 0 <= y1 && y1 < buffer->height &&
        0 <= x2 && x2 < buffer->width && 0 <= y2 && y2 < buffer->height
    );
    float dx = x2 - x1, dy = y2 - y1;
    int steps = abs(dx) >= abs(dy) ? abs(dx) : abs(dy); //To avoid skipping dots

    dx = dx / steps; dy = dy / steps;
    float x = x1, y = y1;
    for (int i = 0; i < steps; i++) {
        buffer_draw_pixel(buffer, (int)x, (int)y, color);
        x += dx;
        y += dy;
    }
}

void buffer_draw_triangle(FrameBuffer *buffer, Triangle triangle, Color color) {
    int x1 = triangle.p1.x, y1 = triangle.p1.y;
    int x2 = triangle.p2.x, y2 = triangle.p2.y;
    int x3 = triangle.p3.x, y3 = triangle.p3.y;
    assert(buffer != NULL && 
        0 <= x1 && x1 < buffer->width && 0 <= y1 && y1 < buffer->height &&
        0 <= x2 && x2 < buffer->width && 0 <= y2 && y2 < buffer->height &&
        0 <= x3 && x3 < buffer->width && 0 <= y3 && y3 < buffer->height
    );

    buffer_draw_line(buffer, x1, y1, x2, y2, color);
    buffer_draw_line(buffer, x2, y2, x3, y3, color);
    buffer_draw_line(buffer, x3, y3, x1, y1, color);
}

double signed_triangle_area(int ax, int ay, int bx, int by, int cx, int cy) {
    return .5*((by-ay)*(bx+ax) + (cy-by)*(cx+bx) + (ay-cy)*(ax+cx));
}
void buffer_draw_filled_triangle(FrameBuffer *buffer, Triangle triangle, Color color) {
    int ax = triangle.p1.x; int ay = triangle.p1.y;
    int bx = triangle.p2.x; int by = triangle.p2.y;
    int cx = triangle.p3.x; int cy = triangle.p3.y;
    
    int min_x = min(min(ax, bx), cx); int min_y = min(min(ay, by), cy);
    int max_x = max(max(ax, bx), cx); int max_y = max(max(ay, by), cy);
    double total_area = signed_triangle_area(ax, ay, bx, by, cx, cy);

    #pragma omp parallel for
    for (int x = min_x; x <= max_x; x++) {
        for (int y = min_y; y <= max_y; y++) {
            double alpha = signed_triangle_area(x, y, bx, by, cx, cy) / total_area;
            double beta  = signed_triangle_area(x, y, cx, cy, ax, ay) / total_area;
            double gamma = signed_triangle_area(x, y, ax, ay, bx, by) / total_area;
            if (alpha < 0 || beta < 0 || gamma < 0) continue; // negative barycentric coordinate => the pixel is outside the triangle
            buffer_draw_pixel(buffer, x, y, color);
        }
    }

}

void swap(int *x1, int *y1, int *x2, int *y2) {
    int temp_x = *x1; *x1 = *x2; *x2 = temp_x;
    int temp_y = *y1; *y1 = *y2; *y2 = temp_y;
}
void buffer_draw_filled_triangle1(FrameBuffer *buffer, Triangle triangle, Color color) {
    int x1 = triangle.p1.x, y1 = triangle.p1.y;
    int x2 = triangle.p2.x, y2 = triangle.p2.y;
    int x3 = triangle.p3.x, y3 = triangle.p3.y;
    assert(buffer != NULL && 
        0 <= x1 && x1 < buffer->width && 0 <= y1 && y1 < buffer->height &&
        0 <= x2 && x2 < buffer->width && 0 <= y2 && y2 < buffer->height &&
        0 <= x3 && x3 < buffer->width && 0 <= y3 && y3 < buffer->height
    );

    // 1 : sort by y coordinates
    if (y1 > y2) {swap(&x1, &y1, &x2, &y2);}
    if (y1 > y3) {swap(&x1, &y1, &x3, &y3);}
    if (y2 > y3) {swap(&x2, &y2, &x3, &y3);}

    // 2 : Rasterize the top part [y1; y2]
    float m2 = (x2 - x1) / (float) (y2 - y1);
    float m3 = (x3 - x1) / (float) (y3 - y1);

    float u2 = x1; // The one that goes to y2
    float u3 = x1; // The one that goes to y3

    for (int y = y1; y <= y2; y++) {
        buffer_draw_line(buffer, u2, y, u3, y, color);

        u2 += m2;
        u3 += m3;
    }
    // 3 : Rasterise the top part [y2; y3]
    float n1 = (x1 - x3) / (float) (y1 - y3);
    float n2 = (x2 - x3) / (float) (y2 - y3);

    float v1 = x3; // The one that goes to y1
    float v2 = x3; // The one that goes to y2

    for (int y = y3; y >= y2; y--) {
        buffer_draw_line(buffer, (int)v1, y, (int)v2, y, color);
        v1 -= n1;
        v2 -= n2;
    }
}