#ifndef FACTION_VK_DEVICE_H
#define FACTION_VK_DEVICE_H


#include <volk/volk.h>

#include "result.h"


typedef struct f_darray f_darray;


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
f_darray* f_get_vk_p_devices(VkInstance _instance, f_res* _res);
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // FACTION_VK_DEVICE_H
