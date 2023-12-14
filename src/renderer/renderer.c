#include <stddef.h>
#include <stdint.h>

#include <volk/volk.h>
#include <GLFW/glfw3.h>

#include "renderer/renderer.h"
#include "utils/darray.h"


struct f_renderer {
    VkInstance instance;
};


VkInstance f_vk_create_instance(const char* _title);
void f_vk_destroy_instance(VkInstance _instance);
f_darray* f_get_vk_req_instance_exts(void);


VkInstance f_vk_create_instance(const char* _title) {
    VkApplicationInfo app_info = {0};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = _title;
    app_info.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
    app_info.pEngineName = "Faction Engine";
    app_info.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
    app_info.apiVersion = VK_API_VERSION_1_3;

    f_darray* arr = f_get_vk_req_instance_exts();
    if(!arr) return NULL;

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
    VkInstance instance = NULL;
    if(vkCreateInstance(&create_info, NULL, &instance) != VK_SUCCESS) {
        return NULL;
    }

    f_destroy_darray(arr);
    return instance;
}

void f_vk_destroy_instance(VkInstance _instance) {
    vkDestroyInstance(_instance, NULL); 
}

f_darray* f_get_vk_req_instance_exts(void) {
    uint32_t glfw_count = 0;
    const char** glfw_exts = glfwGetRequiredInstanceExtensions(&glfw_count);
    if(!glfw_exts) return NULL;

    f_darray* arr = NULL;
    f_create_darray(&arr, 0);
    if(!arr) return NULL;

    f_res res;
    for(uint32_t i = 0; i < glfw_count; i++) {
        res = f_darray_push(arr, (void*) glfw_exts[i]);
        if(res != F_SUCCESS) {
            f_destroy_darray(arr);
            return NULL;
        }
    }

#ifdef __APPLE__
    f_darray_push(arr, VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
#endif // __APPLE__
    return arr;
}
