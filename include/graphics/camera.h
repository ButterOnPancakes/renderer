#ifndef CAMERA_H
#define CAMERA_H

#include "utils/math_utils.h"
#include "transform.h"

typedef struct {
    Transform transform; // Look in the z direction

    float fov;
    float aspect_ratio; // Width / height
    float depth_near; // Nearest clipping plane
    float depth_far; // Farthest clipping plane
} Camera;

void camera_init(Camera *cam);
void camera_destroy(Camera *cam);

void camera_update(Camera *cam, double time);

Mat4 mat4_model(Transform transform);
Mat4 mat4_view(Camera *camera);
Mat4 mat4_projection(Camera *camera);
Vec4 vec4_ndc(Vec4 v);
// Assume that the vector is normalized
Mat4 mat4_viewport(int x, int y, int width, int height);

#endif