#include <stdlib.h>

#include "utils/darray.h"


struct f_darray {
    void** data;
    size_t cap;
    size_t size;
};


f_res f_create_darray(f_darray** _arr, size_t _soft_cap) {
    if(!_arr) return F_ERR_PARAMS;

    f_darray* arr = (f_darray*) malloc(sizeof(*arr));
    if(!arr) return F_ERR_MEMORY;

    arr->data = NULL;
    if(_soft_cap) {
        arr->data = (void**) malloc(sizeof(*arr->data) * _soft_cap);
        if(!arr->data) {
            free(arr);
            return F_ERR_MEMORY;
        }
    }

    arr->cap = _soft_cap;
    arr->size = 0;
    *_arr = arr;
    return F_SUCCESS;
}

void f_destroy_darray(f_darray* _arr) {
    if(!_arr) return;

    if(_arr->data) free(_arr->data);

    free(_arr);
}
