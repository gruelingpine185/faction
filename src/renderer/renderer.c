#include <stdlib.h>
#if F_DEBUG_MODE
#include <stdio.h>
#endif // F_DEBUG_MODE

#include <volk/volk.h>
#include <GLFW/glfw3.h>

#include "utils/darray.h"
#include "renderer/vk_instance.h"
#include "renderer/vk_device.h"
#include "renderer/window.h"
#include "renderer/renderer.h"


struct f_renderer {
    VkInstance instance;
    VkPhysicalDevice* devices;
    uint32_t device_count;
};


f_res f_create_renderer(f_renderer** _renderer, f_window* _win) {
    if(!_renderer || !_win) return F_ERR_PARAMS;

    if(volkInitialize() != VK_SUCCESS) return F_ERR_INTERNAL;

    f_renderer* renderer = (f_renderer*) malloc(sizeof(*renderer));
    if(!renderer) return F_ERR_MEMORY;

    f_res res;
    renderer->instance = f_vk_create_instance(f_get_window_title(_win, NULL),
                                                &res);
    if(res != F_SUCCESS) {
        free(renderer);
        return res;
    }

    volkLoadInstance(renderer->instance);
    renderer->devices = f_get_vk_p_devices(renderer->instance,
                                            &renderer->device_count,
                                            &res);
    if(!renderer->devices) {
        f_destroy_renderer(renderer);
        return res;
    }

    return F_SUCCESS;
}

void f_destroy_renderer(f_renderer* _renderer) {
    if(!_renderer) return;

    if(_renderer->devices) free(_renderer->devices);
    if(_renderer->instance) f_vk_destroy_instance(_renderer->instance);

    free(_renderer);
}
