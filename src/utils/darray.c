#include <stdlib.h>

#include "utils/darray.h"


#ifndef F_DARR_BLOCK_SIZE
    #define F_DARR_CHUNK_SIZE 16
#endif // F_DARR_BLOCK_SIZE


struct f_darray {
    void** data;
    size_t cap;
    size_t size;
};


int f_darray_is_full(const f_darray* _arr, f_res* _res);
f_res f_realloc_darray(f_darray* _arr);


int f_darray_is_full(const f_darray* _arr, f_res* _res) {
    if(!_arr) {
        if(_res) {
            *_res = F_ERR_PARAMS;
            return 0;
        }
    }

    if(_res) *_res = F_SUCCESS;

    return (_arr->size == _arr->cap);
}

f_res f_realloc_darray(f_darray* _arr) {
    if(!_arr) return F_ERR_PARAMS;

    size_t new_cap = (!_arr->cap)?
        F_DARR_CHUNK_SIZE: _arr->cap * 2;
    void** new_data = (!_arr->data)?
        (void**) malloc(sizeof(*_arr->data) * new_cap):
        (void**) realloc(_arr->data, new_cap);

    if(!new_data) return F_ERR_MEMORY;

    _arr->cap = new_cap;
    _arr->data = new_data;
    return F_SUCCESS;
}

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

int f_darray_is_empty(const f_darray* _arr, f_res* _res) {
    if(!_arr) {
        if(_res) *_res = F_ERR_PARAMS;

        return 0;
    }

    if(*_res) *_res = F_SUCCESS;
    return (!_arr->size || !_arr->data || !_arr->cap); 
}
