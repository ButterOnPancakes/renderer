#include "minifb.h"

Window *window_open(const char *title, unsigned width, unsigned height) {
    return mfb_open_ex(title, width, height, MFB_WF_RESIZABLE);
}

bool window_update_buffer(Window *window, void *renderer) {
    mfb_update_state state = mfb_update(window, renderer);
    return state == MFB_STATE_OK;
}
bool window_update_window(Window *window) {
    return mfb_wait_sync(window);
}

bool is_key_pressed(Window *window, mfb_key key) {
    const uint8_t *key_buffer = mfb_get_key_buffer(window);
    return key_buffer[key] == 1;
}