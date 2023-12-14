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
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // FACTION_DARRAY_H
