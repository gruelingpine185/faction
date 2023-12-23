#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#if F_DEBUG_MODE
#include <stdio.h>
#endif // F_DEBUG_MODE

#include <volk/volk.h>
#include <GLFW/glfw3.h>

#include "renderer/window.h"
#include "renderer/renderer.h"
#include "renderer/setup.h"
#include "utils/darray.h"


struct f_renderer {
    VkInstance instance;
    f_darray* devices;
};


VkInstance f_vk_create_instance(const char* _title, f_res* _res);
void f_vk_destroy_instance(VkInstance _instance);
f_darray* f_get_vk_req_instance_exts(f_res* _res);
f_darray* f_get_vk_v_layers(f_res* _res);
f_darray* f_get_vk_p_devices(VkInstance _instance, f_res* _res);
int f_vk_check_supported_v_layers(const f_darray* _layers, f_res* _res);


VkInstance f_vk_create_instance(const char* _title, f_res* _res) {
    f_res res;
    f_darray* exts = f_get_vk_req_instance_exts(&res);
    F_CHECK(exts, _res, res, NULL)

#if F_DEBUG_MODE
    f_darray* v_layers = f_get_vk_v_layers(&res);
    if(!v_layers) {
        if(_res) *_res = res;

        f_destroy_darray(exts);
        return NULL;
    }

    if(!f_vk_check_supported_v_layers(v_layers, &res)) {
        if(_res) *_res = res;

        f_destroy_darray(exts);
        f_destroy_darray(v_layers);
        return NULL;
    }
#endif // F_DEBUG_MODE
    VkApplicationInfo app_info = f_vk_setup_app_info(_title);
#if F_DEBUG_MODE
    VkInstanceCreateInfo create_info = f_vk_setup_create_info(&app_info,
                                                                exts,
                                                                v_layers,
                                                                &res);
#else
    VkInstanceCreateInfo create_info = f_vk_setup_create_info(&app_info,
                                                                exts,
                                                                NULL,
                                                                &res);
#endif // F_DEBUG_MODE
    if(res != F_SUCCESS) {
        if(_res) *_res = res;

        f_destroy_darray(v_layers);
        f_destroy_darray(exts);
        return NULL;
    }
#if F_DEBUG_MODE
    printf("Required Extensions:\n");
    for(size_t i = 0; i < f_get_darray_size(exts, NULL); i++) {
        printf("    %s\n", (const char*) f_get_darray_at(exts, i, NULL));
    }

    printf("Enabled Layers:\n");
    for(size_t i = 0; i < f_get_darray_size(v_layers, NULL); i++) {
        printf("    %s\n", (const char*) f_get_darray_at(v_layers, i, NULL));
    }
#endif // F_DEBUG_MODE
    VkInstance instance = NULL;
    VkResult vk_res = vkCreateInstance(&create_info, NULL, &instance);
    if(vk_res != VK_SUCCESS) {
        f_destroy_darray(v_layers);
        f_destroy_darray(exts);
        return NULL;
    }

    f_destroy_darray(exts);
#if F_DEBUG_MODE
    f_destroy_darray(v_layers);
#endif // F_DEBUG_MODE
    if(_res) *_res = F_SUCCESS;

    return instance;
}

f_darray* f_get_vk_req_instance_exts(f_res* _res) {
    uint32_t glfw_count = 0;
    const char** glfw_exts = glfwGetRequiredInstanceExtensions(&glfw_count);
    F_CHECK(glfw_exts, _res, F_ERR_INTERNAL, NULL)

    f_darray* arr = NULL;
    f_res res = f_create_darray(&arr, 0);
    F_CHECK(arr, _res, res, NULL)

    res = f_darray_push_list(arr, (void**) glfw_exts, glfw_count);
    if(res != F_SUCCESS) {
        if(_res) *_res = res;

        f_destroy_darray(arr);
        return NULL;
    }
#ifdef __APPLE__
    res = f_darray_push(arr, VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
#endif // __APPLE__
#ifdef F_DEBUG_MODE
    res = f_darray_push(arr, VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif // F_DEBUG_MODE
    return arr;
}

f_darray* f_get_vk_v_layers(f_res* _res) {
    f_darray* layers = NULL;
    f_res res = f_create_darray(&layers, 0);
    F_CHECK(layers, _res, res, NULL)

    res = f_darray_push(layers, "VK_LAYER_KHRONOS_validation");
    if(res != F_SUCCESS) {
        if(_res) *_res = res;

        f_destroy_darray(layers);
        return NULL;
    }

    if(_res) *_res = F_SUCCESS;

    return layers;
}

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

void f_vk_destroy_instance(VkInstance _instance) {
    vkDestroyInstance(_instance, NULL); 
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
