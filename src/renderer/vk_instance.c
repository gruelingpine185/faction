#include <stddef.h>
#include <stdint.h>
#include <string.h>
#if F_DEBUG_MODE
#include <stdio.h>
#endif // F_DEBUG_MODE

#include <GLFW/glfw3.h>

#include "utils/darray.h"
#include "renderer/renderer.h"
#include "renderer/vk_instance.h"


VkApplicationInfo f_vk_setup_app_info(const char* _title);
VkInstanceCreateInfo f_vk_setup_create_info(const VkApplicationInfo* _app_info,
                                                const f_darray* _exts,
                                                const f_darray* _layers,
                                                f_res* _res);


VkApplicationInfo f_vk_setup_app_info(const char* _title) {
    VkApplicationInfo app_info = {0};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = _title;
    app_info.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
    app_info.pEngineName = "Faction Engine";
    app_info.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
    app_info.apiVersion = VK_API_VERSION_1_3;
    return app_info;
}

VkInstanceCreateInfo f_vk_setup_create_info(const VkApplicationInfo* _app_info,
                                                const f_darray* _exts,
                                                const f_darray* _layers,
                                                f_res* _res) {
    VkInstanceCreateInfo create_info = {0};
    F_CHECK(_exts, _res, F_ERR_PARAMS, create_info);
#if !F_DEBUG_MODE
    F_CHECK(_layers == NULL, _res, F_ERR_PARAMS, create_info);
#endif // F_DEBUG_MODE

    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
#ifdef __APPLE__
    create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif // __APPLE__
    create_info.pApplicationInfo = _app_info;
    create_info.enabledExtensionCount =
        (uint32_t) f_get_darray_size(_exts, NULL);
    create_info.ppEnabledExtensionNames =
        (const char**) f_get_darray_data(_exts, NULL);
#if F_DEBUG_MODE
    create_info.enabledLayerCount =
        (uint32_t) f_get_darray_size(_layers, NULL);
    create_info.ppEnabledLayerNames =
        (const char**) f_get_darray_data(_layers, NULL);
#endif // F_DEBUG_MODE
    if(_res) *_res = F_SUCCESS;

    return create_info;
}

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
#if F_DEBUG_MODE
        f_destroy_darray(v_layers);
#endif // F_DEBUG_MODE
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
#if F_DEBUG_MODE
        f_destroy_darray(v_layers);
#endif // F_DEBUG_MODE
        f_destroy_darray(exts);
        return NULL;
    }

#if F_DEBUG_MODE
    f_destroy_darray(v_layers);
#endif // F_DEBUG_MODE
    f_destroy_darray(exts);
    if(_res) *_res = F_SUCCESS;

    return instance;
}

void f_vk_destroy_instance(VkInstance _instance) {
    vkDestroyInstance(_instance, NULL); 
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
    if(_res) *_res = res;
    if(res != F_SUCCESS) {
        f_destroy_darray(arr);
        return NULL;
    }

    return arr;
}

f_darray* f_get_vk_v_layers(f_res* _res) {
    f_darray* layers = NULL;
    f_res res = f_create_darray(&layers, 0);
    F_CHECK(layers, _res, res, NULL)

    res = f_darray_push(layers, "VK_LAYER_KHRONOS_validation");
    if(_res) *_res = res;
    if(res != F_SUCCESS) {
        f_destroy_darray(layers);
        return NULL;
    }

    return layers;
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
