#include "minifb.h"

#include "graphics/renderer.h"
#include "graphics/camera.h"
#include "graphics/mesh.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH  256
#define HEIGHT 256

void msleep(long msec) {
    struct timespec ts;

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    nanosleep(&ts, &ts);
}

int main() {
    srand(time(NULL));
    Window *window = window_open("Computer Graphics goes brrrr", WIDTH, HEIGHT);
    if (!window) return -1;

    Renderer renderer; renderer_init(&renderer, WIDTH, HEIGHT);
    Camera cam; camera_init(&cam);
    cam.transform.pos.z = 5;

    Mesh mesh; mesh_load(&mesh, "assets/car.obj");
    Transform transform = {
        .pos = (Vec3) {0, 0, 0},
        .rot = (Vec3) {0, 0, 0},
        .size = (Vec3) {1, 1, 1}
    };

    double time = 0;
    float speed = 5;
    float rot_speed = 2;
    double DT = 1./60.;
    while (true) {
        Vec3 dir = {0, 0, 0};
        if(is_key_pressed(window, MFB_KB_KEY_A)) cam.transform.rot.y += rot_speed * DT;
        if(is_key_pressed(window, MFB_KB_KEY_E)) cam.transform.rot.y -= rot_speed * DT;
        if(is_key_pressed(window, MFB_KB_KEY_Z)) dir.z -= speed * DT;
        if(is_key_pressed(window, MFB_KB_KEY_S)) dir.z += speed * DT;
        if(is_key_pressed(window, MFB_KB_KEY_Q)) dir.x -= speed * DT;
        if(is_key_pressed(window, MFB_KB_KEY_D)) dir.x += speed * DT;
        if(is_key_pressed(window, MFB_KB_KEY_LEFT_SHIFT)) dir.y -= speed * DT;
        if(is_key_pressed(window, MFB_KB_KEY_SPACE)) dir.y += speed * DT;

        Mat3 rotation_x = mat3_rotate_x(cam.transform.rot.x);
        Mat3 rotation_y = mat3_rotate_y(cam.transform.rot.y);
        Mat3 rotation_z = mat3_rotate_z(cam.transform.rot.z);
        Mat3 rotation = mat3_mul(rotation_y, mat3_mul(rotation_x, rotation_z));

        cam.transform.pos = vec3_add(cam.transform.pos, mat3_vec3_mul(rotation, dir));

        renderer_clear(&renderer, 0x000000);

        renderer_draw_mesh(&renderer, &mesh, transform, &cam, 0xFF0000);
        
        if (!window_update_buffer(window, renderer.frame_buffer)) break;
        if (!window_update_window(window)) break;
        
        msleep(1e3 * DT);
        time += DT;
    }
    window = NULL;

    renderer_destroy(&renderer);
    mesh_destroy(&mesh);
    camera_destroy(&cam);

    return 0;
}