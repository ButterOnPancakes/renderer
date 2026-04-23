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
    min_x = max_x = model->triangles[0].p1.x;
    min_y = max_y = model->triangles[0].p1.y;
    min_z = max_z = model->triangles[0].p1.z;

    // 2. Find the actual min/max across all triangles
    for (size_t i = 0; i < model->size; i++) {
        Point p[3] = {model->triangles[i].p1, model->triangles[i].p2, model->triangles[i].p3};
        for (int j = 0; j < 3; j++) {
            if (p[j].x < min_x) min_x = p[j].x; if (p[j].x > max_x) max_x = p[j].x;
            if (p[j].y < min_y) min_y = p[j].y; if (p[j].y > max_y) max_y = p[j].y;
            if (p[j].z < min_z) min_z = p[j].z; if (p[j].z > max_z) max_z = p[j].z;
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
    // Formula: P_new = (P_old - Center) / (Max_Dimension / 2)
    // This puts the model in a -1 to 1 range.
    for (size_t i = 0; i < model->size; i++) {
        Point *p[3] = {&model->triangles[i].p1, &model->triangles[i].p2, &model->triangles[i].p3};
        for (int j = 0; j < 3; j++) {
            p[j]->x = (p[j]->x - center_x) / (max_diff / 2.0);
            p[j]->y = (p[j]->y - center_y) / (max_diff / 2.0);
            p[j]->z = (p[j]->z - center_z) / (max_diff / 2.0);
        }
    }
}

void obj_model_load(ObjModel *model, const char *filepath) {
    FILE* source = fopen(filepath, "r");
    if (!source) return;

    // Use the dynamic array logic you already wrote (it's good!)
    size_t p_cap = 10, p_count = 0;
    Point *points = malloc(p_cap * sizeof(Point));

    size_t t_cap = 10, t_count = 0;
    Triangle *triangles = malloc(t_cap * sizeof(Triangle));

    char line[256];
    while (fgets(line, sizeof(line), source)) {
        // 1. Skip comments
        if (line[0] == '#') continue;

        // 2. Parse Vertices
        if (line[0] == 'v' && line[1] == ' ') {
            if (p_count >= p_cap) {
                p_cap *= 2;
                points = realloc(points, p_cap * sizeof(Point));
            }
            sscanf(line, "v %lf %lf %lf", &points[p_count].x, &points[p_count].y, &points[p_count].z);
            p_count++;
        } 
        
        // 3. Parse Faces (Ignore vt and vn for now to keep it simple)
        else if (line[0] == 'f' && line[1] == ' ') {
            if (t_count >= t_cap) {
                t_cap *= 2;
                triangles = realloc(triangles, t_cap * sizeof(Triangle));
            }

            int v1, vt1, vn1, v2, vt2, vn2, v3, vt3, vn3;
            // This sscanf handles the f v/vt/vn format
            int matches = sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", 
                                &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);
            
            if (matches == 9) {
                // .obj indices start at 1, so we subtract 1
                triangles[t_count].p1 = points[v1 - 1];
                triangles[t_count].p2 = points[v2 - 1];
                triangles[t_count].p3 = points[v3 - 1];
                triangles[t_count].color = rand()%0xFFFFFF;
                t_count++;
            }
        }
    }

    fclose(source);
    free(points);

    model->size = t_count;
    model->triangles = triangles;

    normalize_model(model);
}

void obj_model_destroy(ObjModel *obj) {
    assert(obj != NULL && obj->triangles != NULL);
    free(obj->triangles);
}

void buffer_draw_obj(FrameBuffer *buffer, ObjModel *obj, Color color) {
    assert(buffer != NULL && obj != NULL && obj->triangles != NULL && obj->size > 0);

    double scale = 2.001;
    for(size_t i = 0; i < obj->size; i++) {
        int x1 = (1 - obj->triangles[i].p1.x) * buffer->width / scale;
        int y1 = (1 - obj->triangles[i].p1.y) * buffer->height / scale;
        int x2 = (1 - obj->triangles[i].p2.x) * buffer->width / scale;
        int y2 = (1 - obj->triangles[i].p2.y) * buffer->height / scale;
        int x3 = (1 - obj->triangles[i].p3.x) * buffer->width / scale;
        int y3 = (1 - obj->triangles[i].p3.y) * buffer->height / scale;
        buffer_draw_filled_triangle(buffer, x1, y1, x2, y2, x3, y3, obj->triangles[i].color);
    }
}