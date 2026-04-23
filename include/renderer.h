#ifndef RENDERER_H
#define RENDERER_H

#include <stdint.h>

typedef uint32_t Color;
typedef struct {
    int width, height;
    Color *buffer;
} FrameBuffer;

void buffer_init(FrameBuffer *buffer, int width, int height);
void buffer_destroy(FrameBuffer *buffer);

void buffer_clear(FrameBuffer *buffer, Color color);
void buffer_draw_pixel(FrameBuffer *buffer, int x, int y, Color color);

void buffer_draw_line(FrameBuffer *buffer, int x1, int y1, int x2, int y2, Color color);
void buffer_draw_triangle(FrameBuffer *buffer, int x1, int y1, int x2, int y2, int x3, int y3, Color color);
void buffer_draw_filled_triangle(FrameBuffer *buffer, int x1, int y1, int x2, int y2, int x3, int y3, Color color);

#endif