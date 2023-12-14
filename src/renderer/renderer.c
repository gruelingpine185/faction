#include <stddef.h>
#include <stdint.h>
#include <volk/volk.h>
#include <GLFW/glfw3.h>

#include "renderer/renderer.h"
#include "utils/darray.h"


struct f_renderer {
    VkInstance instance;
};
f_darray* f_get_vk_req_instance_exts(void);
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
