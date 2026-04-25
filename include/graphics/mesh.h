#ifndef MESH_MANAGER_H
#define MESH_MANAGER_H

#include "utils/color_utils.h"

#include <stdlib.h>

typedef struct {
    float x, y, z;
} Vertex;

typedef struct {
    Vertex v1, v2, v3;
    Color color;
} Face;

typedef struct {
    size_t size;
    Face *faces;
} Mesh;

void mesh_load(Mesh *mesh, const char *filepath);
void mesh_destroy(Mesh *mesh);

#endif