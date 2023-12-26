#include "utils/darray.h"
#include "renderer/vk_device.h"


f_darray* f_get_vk_p_devices(VkInstance _instance, f_res* _res) {
    uint32_t count = 0;
    vkEnumeratePhysicalDevices(_instance, &count, NULL);
    F_CHECK(count, _res, F_ERR_NO_SUPPORTED_GPU, NULL)

    f_darray* devices = NULL;
    f_res res = f_create_darray(&devices, count);
    F_CHECK(res == F_SUCCESS, _res, res, NULL);

    VkPhysicalDevice devices_arr[count];
    vkEnumeratePhysicalDevices(_instance, &count, devices_arr);
    res = f_darray_push_list(devices, (void**) devices_arr, count);
    if(res != F_SUCCESS) {
        if(_res) *_res = res;

        f_destroy_darray(devices);
        return NULL;
    }

    if(_res) *_res = F_SUCCESS;

    return devices;
}
