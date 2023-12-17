#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#if !NDEBUG
    #include <stdio.h>
#endif // NDEBUG

#include <volk/volk.h>
#include <GLFW/glfw3.h>

#include "renderer/window.h"
#include "renderer/renderer.h"
#include "utils/darray.h"


struct f_renderer {
    VkInstance instance;
};


VkInstance f_vk_create_instance(const char* _title, f_res* _res);
void f_vk_destroy_instance(VkInstance _instance);
f_darray* f_get_vk_req_instance_exts(f_res* _res);
f_darray* f_get_vk_v_layers(f_res* _res);


VkInstance f_vk_create_instance(const char* _title, f_res* _res) {
    VkApplicationInfo app_info = {0};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = _title;
    app_info.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
    app_info.pEngineName = "Faction Engine";
    app_info.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
    app_info.apiVersion = VK_API_VERSION_1_3;

    f_res res;
    f_darray* arr = f_get_vk_req_instance_exts(&res);
    F_CHECK(arr, &res, res, NULL)

    const char* v_layers[] = {"VK_LAYER_KHRONOS_validation"};
    VkInstanceCreateInfo create_info = {0};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
#ifdef __APPLE__
    create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif // __APPLE__
    create_info.pApplicationInfo = &app_info;
    create_info.enabledExtensionCount =
        (uint32_t) f_get_darray_size(arr, NULL);
    create_info.ppEnabledExtensionNames =
        (const char**) f_get_darray_data(arr, NULL);
    create_info.enabledLayerCount = (sizeof(v_layers) / sizeof(*v_layers));
    create_info.ppEnabledLayerNames = v_layers; 

#if !NDEBUG
    printf("Required Extensions:\n");
    for(size_t i = 0; i < create_info.enabledExtensionCount; i++) {
        printf("    %s\n", (const char*) f_get_darray_at(arr, i, NULL));
    }

    printf("Enabled Layers:\n");
    for(size_t i = 0; i < create_info.enabledLayerCount; i++) {
        printf("    %s\n", create_info.ppEnabledLayerNames[i]);
    }
#endif // NDEBUG
    VkInstance instance = NULL;
    VkResult vk_res = vkCreateInstance(&create_info, NULL, &instance);
    F_CHECK(vk_res == VK_SUCCESS, _res, F_ERR_INTERNAL, NULL);

    f_destroy_darray(arr);
    return instance;
}

f_darray* f_get_vk_req_instance_exts(f_res* _res) {
    uint32_t glfw_count = 0;
    const char** glfw_exts = glfwGetRequiredInstanceExtensions(&glfw_count);
    F_CHECK(glfw_exts, _res, F_ERR_INTERNAL, NULL)

    f_darray* arr = NULL;
    f_res res = f_create_darray(&arr, 0);
    F_CHECK(arr, _res, res, NULL)

    for(uint32_t i = 0; i < glfw_count; i++) {
        res = f_darray_push(arr, (void*) glfw_exts[i]);
        if(res != F_SUCCESS) {
            f_destroy_darray(arr);
            return NULL;
        }
    }

#ifdef __APPLE__
    res = f_darray_push(arr, VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
    if(res != F_SUCCESS) {
        f_destroy_darray(arr);
        return NULL;
    }
#endif // __APPLE__
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

    return layers;
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
    if(!renderer->instance) {
        free(renderer);
        return res;
    }

    volkLoadInstance(renderer->instance);
    return F_SUCCESS;
}

void f_destroy_renderer(f_renderer* _renderer) {
    if(!_renderer) return;

    if(_renderer->instance) f_vk_destroy_instance(_renderer->instance);

    free(_renderer);
}
