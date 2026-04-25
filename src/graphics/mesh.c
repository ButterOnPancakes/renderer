#include "graphics/mesh.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void normalize_mesh(Mesh *mesh) {
    if (mesh->size == 0) return;

    // 1. Initialize bounds with the first point of the first triangle
    double min_x, max_x, min_y, max_y, min_z, max_z;
    min_x = max_x = mesh->faces[0].v1.x;
    min_y = max_y = mesh->faces[0].v1.y;
    min_z = max_z = mesh->faces[0].v1.z;

    // 2. Find the actual min/max across all faces
    for (size_t i = 0; i < mesh->size; i++) {
        Vertex v[3] = {mesh->faces[i].v1, mesh->faces[i].v2, mesh->faces[i].v3};
        for (int j = 0; j < 3; j++) {
            if (v[j].x < min_x) min_x = v[j].x; if (v[j].x > max_x) max_x = v[j].x;
            if (v[j].y < min_y) min_y = v[j].y; if (v[j].y > max_y) max_y = v[j].y;
            if (v[j].z < min_z) min_z = v[j].z; if (v[j].z > max_z) max_z = v[j].z;
        }
    }

    // 3. Calculate Center and Scale
    double center_x = (min_x + max_x) / 2.0;
    double center_y = (min_y + max_y) / 2.0;
    double center_z = (min_z + max_z) / 2.0;

    double diff_x = max_x - min_x;
    double diff_y = max_y - min_y;
    double diff_z = max_z - min_z;
    
    // Find the largest dimension to use as the scale factor
    double max_diff = diff_x;
    if (diff_y > max_diff) max_diff = diff_y;
    if (diff_z > max_diff) max_diff = diff_z;

    // 4. Apply transformation to every vertex
    // Formula: v_new = (v_old - Center) / (Max_Dimension / 2)
    // This puts the mesh in a -1 to 1 range.
    for (size_t i = 0; i < mesh->size; i++) {
        Vertex *v[3] = {&mesh->faces[i].v1, &mesh->faces[i].v2, &mesh->faces[i].v3};
        for (int j = 0; j < 3; j++) {
            v[j]->x = (v[j]->x - center_x) / (max_diff / 2.0);
            v[j]->y = (v[j]->y - center_y) / (max_diff / 2.0);
            v[j]->z = (v[j]->z - center_z) / (max_diff / 2.0);
        }
    }
}
void mesh_load(Mesh *mesh, const char *filepath) {
    FILE* source = fopen(filepath, "r");
    assert(source != NULL);

    // Use the dynamic array logic you already wrote (it's good!)
    size_t v_cap = 10, v_count = 0;
    Vertex *vertices = malloc(v_cap * sizeof(Vertex));

    size_t f_cap = 10, f_count = 0;
    Face *faces = malloc(f_cap * sizeof(Face));

    char line[256];
    while (fgets(line, sizeof(line), source)) {
        // 1. Skip comments
        if (line[0] == '#') continue;

        // 2. Parse Vertices
        if (line[0] == 'v' && line[1] == ' ') {
            if (v_count >= v_cap) {
                v_cap *= 2;
                vertices = realloc(vertices, v_cap * sizeof(Vertex));
            }
            sscanf(line, "v %f %f %f", &vertices[v_count].x, &vertices[v_count].y, &vertices[v_count].z);
            v_count++;
        } 
        
        // 3. Parse Faces (Ignore vt and vn for now to keep it simple)
        else if (line[0] == 'f' && line[1] == ' ') {
            if (f_count >= f_cap) {
                f_cap *= 2;
                faces = realloc(faces, f_cap * sizeof(Face));
            }
            int v1, v2, v3;
            // 1. f v/vt/vn (Full)
            // 2. f v/vt    (No normals)
            // 3. f v//vn   (No textures)
            // 4. f v       (Indices only)
            if (
                sscanf(line, "f %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &v1, &v2, &v3) == 3 ||
                sscanf(line, "f %d/%*d %d/%*d %d/%*d",             &v1, &v2, &v3) == 3 ||
                sscanf(line, "f %d//%*d %d//%*d %d//%*d",         &v1, &v2, &v3) == 3 ||
                sscanf(line, "f %d %d %d",                         &v1, &v2, &v3) == 3
            ) {
                // .mesh indices start at 1, so we subtract 1
                faces[f_count].v1 = vertices[v1 - 1];
                faces[f_count].v2 = vertices[v2 - 1];
                faces[f_count].v3 = vertices[v3 - 1];
                faces[f_count].color = rand()%(1 << 24);
                f_count++;
            }
        }
    }

    fclose(source);
    free(vertices);

    mesh->size = f_count;
    mesh->faces = faces;

    printf("Model Loaded with %d vertices and %d faces !\n", v_count, f_count);
    fflush(stdout);

    normalize_mesh(mesh);
}

void mesh_destroy(Mesh *mesh) {
    assert(mesh != NULL && mesh->faces != NULL);
    free(mesh->faces);
}
