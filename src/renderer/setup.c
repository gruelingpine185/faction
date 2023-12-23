#include <volk/volk.h>

#include "utils/darray.h"
#include "renderer/setup.h"


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
