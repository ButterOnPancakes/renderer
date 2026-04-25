#include "utils/color_utils.h"

#include <assert.h>

uint8_t color_red(Color color) {
    uint32_t mask = 255 * (1 << 16);
    return (mask & color) >> 16;
}
uint8_t color_green(Color color) {
    uint32_t mask = 255 * (1 << 8);
    return (mask & color) >> 8;
}
uint8_t color_blue(Color color) {
    uint32_t mask = 255;
    return mask & color;
}
Color color_rgb(uint8_t r, uint8_t g, uint8_t b) {
    return (r << 16) | (g << 8) | b;
}
Color color_grayscale(float gray) {
    assert(0 <= gray && gray <= 1);
    uint8_t c = gray * 255;
    return color_rgb(c, c, c);
}

Color color_shade(Color color, float gray) {
    return color_rgb(color_red(color) * gray, color_green(color) * gray, color_blue(color) * gray);
}