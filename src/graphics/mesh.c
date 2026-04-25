#include "graphics/mesh.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void normalize_mesh(Mesh *mesh) {
    if (mesh->num_vertices == 0) return;

    // 1. Initialize bounds with the first vertex
    double min_x, max_x, min_y, max_y, min_z, max_z;
    min_x = max_x = mesh->vertices[0].x;
    min_y = max_y = mesh->vertices[0].y;
    min_z = max_z = mesh->vertices[0].z;

    // 2. Find the actual min/max across all vertices
    for (size_t i = 0; i < mesh->num_vertices; i++) {
        Vertex v = mesh->vertices[i];
        if (v.x < min_x) min_x = v.x; if (v.x > max_x) max_x = v.x;
        if (v.y < min_y) min_y = v.y; if (v.y > max_y) max_y = v.y;
        if (v.z < min_z) min_z = v.z; if (v.z > max_z) max_z = v.z;
    }

    // 3. Calculate Center and Scale
    double center_x = (min_x + max_x) / 2.0;
    double center_y = (min_y + max_y) / 2.0;
    double center_z = (min_z + max_z) / 2.0;

    double diff_x = max_x - min_x;
    double diff_y = max_y - min_y;
    double diff_z = max_z - min_z;
    
    double max_diff = diff_x;
    if (diff_y > max_diff) max_diff = diff_y;
    if (diff_z > max_diff) max_diff = diff_z;

    // 4. Apply transformation to every vertex
    for (size_t i = 0; i < mesh->num_vertices; i++) {
        mesh->vertices[i].x = (mesh->vertices[i].x - center_x) / (max_diff / 2.0);
        mesh->vertices[i].y = (mesh->vertices[i].y - center_y) / (max_diff / 2.0);
        mesh->vertices[i].z = (mesh->vertices[i].z - center_z) / (max_diff / 2.0);
    }
}

void mesh_load(Mesh *mesh, const char *filepath) {
    FILE* source = fopen(filepath, "r");
    assert(source != NULL);

    size_t v_cap = 10, v_count = 0;
    Vertex *vertices = malloc(v_cap * sizeof(Vertex));

    size_t f_cap = 10, f_count = 0;
    Face *faces = malloc(f_cap * sizeof(Face));

    char line[256];
    while (fgets(line, sizeof(line), source)) {
        if (line[0] == '#') continue;

        if (line[0] == 'v' && line[1] == ' ') {
            if (v_count >= v_cap) {
                v_cap *= 2;
                vertices = realloc(vertices, v_cap * sizeof(Vertex));
            }
            sscanf(line, "v %f %f %f", &vertices[v_count].x, &vertices[v_count].y, &vertices[v_count].z);
            v_count++;
        } 
        else if (line[0] == 'f' && line[1] == ' ') {
            if (f_count >= f_cap) {
                f_cap *= 2;
                faces = realloc(faces, f_cap * sizeof(Face));
            }
            int v1, v2, v3;
            if (
                sscanf(line, "f %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &v1, &v2, &v3) == 3 ||
                sscanf(line, "f %d/%*d %d/%*d %d/%*d",             &v1, &v2, &v3) == 3 ||
                sscanf(line, "f %d//%*d %d//%*d %d//%*d",         &v1, &v2, &v3) == 3 ||
                sscanf(line, "f %d %d %d",                         &v1, &v2, &v3) == 3
            ) {
                faces[f_count].v1 = v1 - 1;
                faces[f_count].v2 = v2 - 1;
                faces[f_count].v3 = v3 - 1;
                faces[f_count].color = rand() % (1 << 24);
                f_count++;
            }
        }
    }

    fclose(source);

    mesh->num_vertices = v_count;
    mesh->vertices = vertices;
    mesh->num_faces = f_count;
    mesh->faces = faces;

    printf("Model Loaded with %zu vertices and %zu faces!\n", v_count, f_count);
    fflush(stdout);

    normalize_mesh(mesh);
}

void mesh_destroy(Mesh *mesh) {
    assert(mesh != NULL);
    if (mesh->vertices) free(mesh->vertices);
    if (mesh->faces) free(mesh->faces);
}
