#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

#include "renderer.h"
#include <stdlib.h>

typedef struct {
    double x, y, z;
} Point;

typedef struct {
    Point p1, p2, p3;
    Color color;
} Triangle;

typedef struct {
    size_t size;
    Triangle *triangles;
} ObjModel;

void obj_model_load(ObjModel *obj, const char *filepath);
void obj_model_destroy(ObjModel *obj);

void buffer_draw_obj(FrameBuffer *buffer, ObjModel *obj, Color color);

#endif