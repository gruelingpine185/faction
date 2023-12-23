#ifndef FACTION_SETUP_H
#define FACTION_SETUP_H


#include "result.h"


typedef struct f_darray f_darray;

typedef struct VkApplicationInfo VkApplicationInfo;
typedef struct VkInstanceCreateInfo VkInstanceCreateInfo;


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
VkApplicationInfo f_vk_setup_app_info(const char* _title);
VkInstanceCreateInfo f_vk_setup_create_info(const VkApplicationInfo* _app_info,
                                                const f_darray* _exts,
                                                const f_darray* _layers,
                                                f_res* _res);
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // FACTION_SETUP_H
