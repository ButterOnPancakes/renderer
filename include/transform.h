#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "utils/math_utils.h"

typedef struct {
    Vec3 pos; // x is right, y is up, z is depth, points to the screen
    Vec3 rot; // pitch, yaw, roll
    Vec3 size;
} Transform;

#endif