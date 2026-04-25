#ifndef MINIFB_H
#define MINIFB_H

#include "MiniFB.h"

#include <stdbool.h>

typedef struct mfb_window Window;

Window *window_open(const char *title, unsigned width, unsigned height);

bool window_update_buffer(Window *window, void *buffer);
bool window_update_window(Window *window);

bool is_key_pressed(Window* window, mfb_key key);

#endif