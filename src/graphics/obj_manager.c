#include "obj_manager.h"

#include "renderer.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void normalize_model(ObjModel *model) {
    if (model->size == 0) return;

    // 1. Initialize bounds with the first point of the first triangle
    double min_x, max_x, min_y, max_y, min_z, max_z;
    min_x = max_x = model->faces[0].v1.x;
    min_y = max_y = model->faces[0].v1.y;
    min_z = max_z = model->faces[0].v1.z;

    // 2. Find the actual min/max across all faces
    for (size_t i = 0; i < model->size; i++) {
        Vertex v[3] = {model->faces[i].v1, model->faces[i].v2, model->faces[i].v3};
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
    // This puts the model in a -1 to 1 range.
    for (size_t i = 0; i < model->size; i++) {
        Vertex *v[3] = {&model->faces[i].v1, &model->faces[i].v2, &model->faces[i].v3};
        for (int j = 0; j < 3; j++) {
            v[j]->x = (v[j]->x - center_x) / (max_diff / 2.0);
            v[j]->y = (v[j]->y - center_y) / (max_diff / 2.0);
            v[j]->z = (v[j]->z - center_z) / (max_diff / 2.0);
        }
    }
}

void obj_model_load(ObjModel *model, const char *filepath) {
    FILE* source = fopen(filepath, "r");
    if (!source) return;

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
                // .obj indices start at 1, so we subtract 1
                faces[f_count].v1 = vertices[v1 - 1];
                faces[f_count].v2 = vertices[v2 - 1];
                faces[f_count].v3 = vertices[v3 - 1];
                f_count++;
            }
        }
    }

    fclose(source);
    free(vertices);

    model->size = f_count;
    model->faces = faces;

    normalize_model(model);
}

void obj_model_destroy(ObjModel *obj) {
    assert(obj != NULL && obj->faces != NULL);
    free(obj->faces);
}

void buffer_draw_obj(FrameBuffer *buffer, ObjModel *obj, Color color) {
    assert(buffer != NULL && obj != NULL && obj->faces != NULL && obj->size > 0);

    double scale = 2.001;
    for (size_t i = 0; i < obj->size; i++) {
        Triangle tri;
        tri.p1.x = (1 - obj->faces[i].v1.x) * buffer->width / scale;
        tri.p1.y = (1 - obj->faces[i].v1.y) * buffer->height / scale;
        tri.p2.x = (1 - obj->faces[i].v2.x) * buffer->width / scale;
        tri.p2.y = (1 - obj->faces[i].v2.y) * buffer->height / scale;
        tri.p3.x = (1 - obj->faces[i].v3.x) * buffer->width / scale;
        tri.p3.y = (1 - obj->faces[i].v3.y) * buffer->height / scale;
        buffer_draw_filled_triangle(buffer, tri, 0x0000FF);
        buffer_draw_triangle(buffer, tri, 0xFF0000);
    }
}