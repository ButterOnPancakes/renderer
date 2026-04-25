#ifndef MESH_MANAGER_H
#define MESH_MANAGER_H

#include "utils/color_utils.h"

#include <stdlib.h>

typedef struct {
    float x, y, z;
} Vertex;

typedef struct {
    int v1, v2, v3; // Indices into Mesh.vertices
    Color color;
} Face;

typedef struct {
    Vertex *vertices;
    size_t num_vertices;
    Face *faces;
    size_t num_faces;
} Mesh;

void mesh_load(Mesh *mesh, const char *filepath);
void mesh_destroy(Mesh *mesh);

#endif
