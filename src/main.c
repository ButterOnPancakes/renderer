#include "minifb.h"

#include "renderer.h"
#include "obj_manager.h"

#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH  800
#define HEIGHT 600

int main() {
    srand(time(NULL));
    Window *window = window_open("Computer Graphics goes brrrr", WIDTH, HEIGHT);
    if (!window) return -1;

    FrameBuffer buffer; buffer_init(&buffer, WIDTH, HEIGHT);

    ObjModel* obj = malloc(sizeof(ObjModel));
    obj_model_load(obj, "assets/stone.obj");
    
    do {
        buffer_clear(&buffer, 0x000000);

        buffer_draw_obj(&buffer, obj, 0xFF0000);
        
        if (!window_update_buffer(window, buffer.buffer)) break;
    } while (window_update_window(window));

    window = NULL;

    buffer_destroy(&buffer);
    obj_model_destroy(obj);
    free(obj);

    return 0;
}