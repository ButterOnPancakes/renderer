#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

#include "renderer.h"

#include <stdlib.h>

typedef struct {
    float x, y, z;
} Vertex;

typedef struct {
    Vertex v1, v2, v3;
} Face;

typedef struct {
    size_t size;
    Face *faces;
} ObjModel;

void obj_model_load(ObjModel *obj, const char *filepath);
void obj_model_destroy(ObjModel *obj);

void buffer_draw_obj(FrameBuffer *buffer, ObjModel *obj, Color color);

#endif