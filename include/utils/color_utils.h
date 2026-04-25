#ifndef COLOR_UTILS_H
#define COLOR_UTILS_H

#include <stdint.h>

typedef uint32_t Color;

#define COLOR_RED 0xFF0000
#define COLOR_GREEN 0x00FF00
#define COLOR_BLUE 0x0000FF

uint8_t color_red(Color color);
uint8_t color_green(Color color);
uint8_t color_blue(Color color);
Color color_rgb(uint8_t r, uint8_t g, uint8_t b);

Color color_grayscale(float gray);
Color color_shade(Color color, float gray);

#endif