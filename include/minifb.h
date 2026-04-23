#ifndef MINIFB_H
#define MINIFB_H

#include "MiniFB.h"

#include <stdbool.h>

typedef struct mfb_window Window;

Window *window_open(const char *title, unsigned width, unsigned height);

bool window_update_buffer(struct mfb_window *window, void *buffer);
bool window_update_window(struct mfb_window *window);

#endif