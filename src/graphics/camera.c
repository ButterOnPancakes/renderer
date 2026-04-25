#include "graphics/camera.h"

#include <math.h>

void camera_init(Camera *cam) {
    cam->transform.pos = (Vec3) {.x = 0, .y = 0, .z = 0};
    cam->transform.rot = (Vec3) {.x = 0, .y = 0, .z = 0};
    cam->transform.size = (Vec3) {.x = 1, .y = 1, .z = 1};
    cam->aspect_ratio = 1;
    cam->depth_near = 0.1;
    cam->depth_far = 100000;
    cam->fov = M_PI / 3;
}
void camera_destroy(Camera *cam) {}

void camera_update(Camera *cam, double time) {
    double dist = 10;
    cam->transform.pos = (Vec3) {.x = 10 * sin(time), .z = 10 * cos(time)};
    cam->transform.rot.y = time;
}

Mat4 mat4_model(Transform t) {
    Mat4 translation = mat4_translate(t.pos.x, t.pos.y, t.pos.z);
    Mat4 rotation_x = mat4_rotate_x(t.rot.x);
    Mat4 rotation_y = mat4_rotate_y(t.rot.y);
    Mat4 rotation_z = mat4_rotate_z(t.rot.z);
    Mat4 scale = mat4_scale(t.size.x, t.size.y, t.size.z);

    Mat4 rotation = mat4_mul(rotation_y, mat4_mul(rotation_x, rotation_z));
    return mat4_mul(translation, mat4_mul(rotation, scale));
}
Mat4 mat4_view(Camera *cam) {
    // Compute mat4_model(cam) inverse
    Mat4 rx = mat4_rotate_x(-cam->transform.rot.x);
    Mat4 ry = mat4_rotate_y(-cam->transform.rot.y);
    Mat4 rz = mat4_rotate_z(-cam->transform.rot.z);
    Mat4 rotation = mat4_mul(rz, mat4_mul(rx, ry));

    Mat4 translation = mat4_translate(
        -cam->transform.pos.x, 
        -cam->transform.pos.y, 
        -cam->transform.pos.z
    );

    return mat4_mul(rotation, translation);
}
Mat4 mat4_projection(Camera *cam) {
    float tf = tan(cam->fov / 2.);
    float f = cam->depth_far;
    float n = cam->depth_near;
    float a = cam->aspect_ratio;

    return (Mat4) {{
        {1./(a * tf), 0, 0, 0},
        {0, 1./tf, 0, 0},
        {0, 0, -(f+n)/(f-n), -2*f*n/(f-n)},
        {0, 0, -1, 0}
    }};
}
Vec4 vec4_ndc(Vec4 v) {
    return (Vec4) {.x = v.x / v.w, .y = v.y / v.w, .z = v.z / v.w, .w = 1};
}
Mat4 mat4_viewport(int x, int y, int w, int h) {
    // x,y,z : [-1, 1]^3
    // y need to be reversed
    return (Mat4) {{
        {w/2., 0, 0, w/2.},
        {0, -h/2., 0, h/2.},
        {0, 0, 1/2., 1/2.},
        {0, 0, 0, 1}
    }};
}
