#ifndef FACTION_VK_DEVICE_H
#define FACTION_VK_DEVICE_H


#include <stdint.h>

#include <volk/volk.h>

#include "result.h"


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
VkPhysicalDevice* f_get_vk_p_devices(VkInstance _instance,
                                        uint32_t* _count,
                                        f_res* _res);
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // FACTION_VK_DEVICE_H
