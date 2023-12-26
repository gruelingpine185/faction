#ifndef FACTION_VK_INSTANCE_H
#define FACTION_VK_INSTANCE_H


#include <volk/volk.h>

#include "result.h"


typedef struct f_darray f_darray;


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
VkInstance f_vk_create_instance(const char* _title, f_res* _res);
void f_vk_destroy_instance(VkInstance _instance);
f_darray* f_get_vk_req_instance_exts(f_res* _res);
f_darray* f_get_vk_v_layers(f_res* _res);
int f_vk_check_supported_v_layers(const f_darray* _layers, f_res* _res);
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // FACTION_VK_INSTANCE_H
