#ifndef RENDERER_H
#define RENDERER_H

#include "geometry.h"
#include <stdint.h>

typedef uint32_t Color;
typedef struct {
    int width, height;
    Color *buffer;
} FrameBuffer;

void buffer_init(FrameBuffer *buffer, int width, int height);
void buffer_destroy(FrameBuffer *buffer);

// 2D stuff
void buffer_clear(FrameBuffer *buffer, Color color);
void buffer_draw_pixel(FrameBuffer *buffer, int x, int y, Color color);
void buffer_draw_line(FrameBuffer *buffer, int x1, int y1, int x2, int y2, Color color);

// 3D stuff
void buffer_draw_triangle(FrameBuffer *buffer, Triangle triangle, Color color);
void buffer_draw_filled_triangle(FrameBuffer *buffer, Triangle triangle, Color color);
void buffer_draw_filled_triangle1(FrameBuffer *buffer, Triangle triangle, Color color);

#endif