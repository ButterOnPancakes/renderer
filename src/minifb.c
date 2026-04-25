#include "minifb.h"

Window *window_open(const char *title, unsigned width, unsigned height) {
    return mfb_open_ex(title, width, height, MFB_WF_RESIZABLE);
}

bool window_update_buffer(struct mfb_window *window, void *renderer) {
    mfb_update_state state = mfb_update(window, renderer);
    return state == MFB_STATE_OK;
}
bool window_update_window(struct mfb_window *window) {
    return mfb_wait_sync(window);
}