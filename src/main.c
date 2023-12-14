#include <stddef.h>

#include "renderer/window.h"


int main(void) {
    f_window* win = NULL;
    f_res res = f_create_window(&win, 600, 600, "hello world");
    if(res != F_SUCCESS) return 1;

    while(f_is_window_open(win)) {
        f_window_poll_events();
    }

    return 0;
}
