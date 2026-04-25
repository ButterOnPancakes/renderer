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

    Mesh mesh; mesh_load(&mesh, "assets/head.obj");
    Transform transform = {
        .pos = (Vec3) {0, 0, 0},
        .rot = (Vec3) {0, 0, 0},
        .size = (Vec3) {1, 1, 1}
    };

    double time = 0;
    while (true) {
        camera_update(&cam, time);

        renderer_clear(&renderer, 0x000000);

        renderer_draw_mesh(&renderer, &mesh, transform, &cam, 0xFF0000);
        
        if (!window_update_buffer(window, renderer.frame_buffer)) break;
        if (!window_update_window(window)) break;
        
        msleep(1e3 / 60.);
        time += 1 / 60.;
    }
    window = NULL;

    renderer_destroy(&renderer);
    mesh_destroy(&mesh);
    camera_destroy(&cam);

    return 0;
}