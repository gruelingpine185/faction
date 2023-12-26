#include <stdlib.h>
#if F_DEBUG_MODE
#include <stdio.h>
#endif // F_DEBUG_MODE

#include <volk/volk.h>
#include <GLFW/glfw3.h>

#include "renderer/window.h"
#include "renderer/renderer.h"


struct f_renderer {
    VkInstance instance;
    f_darray* devices;
};



f_darray* f_get_vk_p_devices(VkInstance _instance, f_res* _res) {
    uint32_t count = 0;
    vkEnumeratePhysicalDevices(_instance, &count, NULL);
    F_CHECK(count, _res, F_ERR_NO_SUPPORTED_GPU, NULL)

    f_darray* devices = NULL;
    f_res res = f_create_darray(&devices, count);
    F_CHECK(res == F_SUCCESS, _res, res, NULL);

    VkPhysicalDevice devices_arr[count];
    vkEnumeratePhysicalDevices(_instance, &count, devices_arr);
    res = f_darray_push_list(devices, (void**) devices_arr, count);
    if(res != F_SUCCESS) {
        if(_res) *_res = res;

        f_destroy_darray(devices);
        return NULL;
    }

    if(_res) *_res = F_SUCCESS;

    return devices;
}

int f_vk_check_supported_v_layers(const f_darray* _layers, f_res* _res) {
    F_CHECK(_layers, _res, F_ERR_PARAMS, 0);

    uint32_t count;
    vkEnumerateInstanceLayerProperties(&count, NULL);
    VkLayerProperties available_layers[count];
    vkEnumerateInstanceLayerProperties(&count, available_layers);
    int is_found = 0;

    for(size_t i = 0; i < f_get_darray_size(_layers, NULL); i++) {
        is_found = 0;
        for(uint32_t j = 0; j < count; j++) {
            if(strcmp(f_get_darray_at(_layers, i, NULL),
                        available_layers[j].layerName) == 0) {
                is_found = 1;
                break;
            }
        }

        if(!is_found) return 0;
    }

    return 1;
}
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
    renderer->devices = f_get_vk_p_devices(renderer->instance, &res);
    if(res != F_SUCCESS) {
        f_destroy_renderer(renderer);
        return res;
    }

    return F_SUCCESS;
}

void f_destroy_renderer(f_renderer* _renderer) {
    if(!_renderer) return;

    if(_renderer->instance) f_vk_destroy_instance(_renderer->instance);
    if(_renderer->devices) f_destroy_darray(_renderer->devices);

    free(_renderer);
}
