#ifndef FACTION_DARRAY_H
#define FACTION_DARRAY_H


#include <stddef.h>

#include "result.h"


typedef struct f_darray f_darray;


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
f_res f_create_darray(f_darray** _arr, size_t _soft_cap);
void f_destroy_darray(f_darray* _arr);
f_res f_darray_push(f_darray* _arr, void* _data);
void* f_darray_pop(f_darray* _arr, f_res* _res);
int f_darray_is_empty(const f_darray* _arr, f_res* _res);
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // FACTION_DARRAY_H
