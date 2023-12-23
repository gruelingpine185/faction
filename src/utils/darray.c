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
    F_CHECK(_arr, _res, F_ERR_PARAMS, 0);

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

f_res f_darray_push(f_darray* _arr, void* _data) {
    if(!_arr || !_data) return F_ERR_PARAMS;

    if(!f_darray_is_empty(_arr, NULL) || f_darray_is_full(_arr, NULL)) {
        f_res res = f_realloc_darray(_arr);
        if(res != F_SUCCESS) return res;
    }

    _arr->data[_arr->size++] = _data;
    return F_SUCCESS;
}

f_res f_darray_push_list(f_darray* _arr, void** _data, size_t _len) {
    if(!_arr || !_data || !_len) return F_ERR_PARAMS;

    f_res res;
    for(size_t i = 0; i < _len; i++) {
        res = f_darray_push(_arr, _data[i]);
        if(res != F_SUCCESS) return res;
    }

    return F_SUCCESS;

}

void* f_darray_pop(f_darray* _arr, f_res* _res) {
    F_CHECK(_arr, _res, F_ERR_PARAMS, NULL);

    if(!f_darray_is_empty(_arr, NULL)) {
        f_res res = f_realloc_darray(_arr);
        F_CHECK(*_res == F_SUCCESS, _res, res, NULL);
    }

    if(_res) *_res = F_SUCCESS;

    return _arr->data[--_arr->size];
}

int f_darray_is_empty(const f_darray* _arr, f_res* _res) {
    F_CHECK(_arr, _res, F_ERR_PARAMS, 0);

    if(_res) *_res = F_SUCCESS;

    return (!_arr->size || !_arr->data || !_arr->cap); 
}

void* f_get_darray_at(const f_darray* _arr, size_t _offset, f_res* _res) {
    F_CHECK(_arr, _res, F_ERR_PARAMS, 0);

    if(f_darray_is_empty(_arr, NULL) || (_offset > _arr->cap)) {
        if(_res) *_res = F_ERR_BOUNDS;

        return NULL;
    }

    return _arr->data[_offset];
}

f_res f_set_darray_at(const f_darray* _arr, size_t _offset, void* _data) {
    if(!_arr || !_data) return F_ERR_PARAMS;
    if(f_darray_is_empty(_arr, NULL) || (_offset > _arr->size)) {
        return F_ERR_BOUNDS;
    }

    _arr->data[_offset] = _data;
    return F_SUCCESS;
}

size_t f_get_darray_size(const f_darray* _arr, f_res* _res) {
    F_CHECK(_arr, _res, F_ERR_PARAMS, 0);

    if(_res) *_res = F_SUCCESS;

    return _arr->size; 
}

void** f_get_darray_data(const f_darray* _arr, f_res* _res) {
    F_CHECK(_arr, _res, F_ERR_PARAMS, NULL);

    if(_res) *_res = F_SUCCESS;

    return _arr->data; 
}
