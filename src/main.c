#include <stddef.h>
#include <stdio.h>

#include "renderer/window.h"
#include "renderer/renderer.h"


int main(void) {
    f_window* win = NULL;
    f_res res = f_create_window(&win, 600, 600, "hello world");
    if(res != F_SUCCESS) {
       printf("Faction: Failed to create window.\nReason: \'%s\'.\n", f_res_to_str(res));
        return 1;
    }

    f_renderer* renderer = NULL;
    res = f_create_renderer(&renderer, win);
    if(res != F_SUCCESS) {
       printf("Faction: Failed to create renderer. Reason: \'%s\'.\n", f_res_to_str(res));
        return 1;
    }

    while(f_is_window_open(win)) {
        f_window_poll_events();
    }

    f_destroy_renderer(renderer);
    f_destroy_window(win);
    return 0;
}
