#include <stddef.h>
#include <stdlib.h>
#if F_DEBUG_MODE
#include <stdio.h>
#endif // F_DEBUG_MODE

#include "renderer/vk_device.h"


VkPhysicalDevice* f_get_vk_p_devices(VkInstance _instance,
                                        uint32_t* _count,
                                        f_res* _res) {
    F_CHECK(_instance, _res, F_ERR_PARAMS, NULL)

    uint32_t count = 0;
    vkEnumeratePhysicalDevices(_instance, &count, NULL);
    F_CHECK(count != 0, _res, F_ERR_NO_SUPPORTED_GPU, NULL);

    VkPhysicalDevice* devices = malloc(sizeof(VkPhysicalDevice) * count);
    F_CHECK(devices, _res, F_ERR_MEMORY, NULL);

    vkEnumeratePhysicalDevices(_instance, &count, devices);
#if F_DEBUG_MODE
    printf("Physical Devices (GPUs):\n");
    for(uint32_t i = 0; i < count; i++) {
        VkPhysicalDeviceProperties prop = {0};
        vkGetPhysicalDeviceProperties(devices[i], &prop);
        printf("    %s\n", prop.deviceName);
    }
#endif // F_DEBUG_MODE
    if(_res) *_res = F_SUCCESS;
    if(_count) *_count = count;
    
    return devices;
}
