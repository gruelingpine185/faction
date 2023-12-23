#ifndef FACTION_RESULT_H
#define FACTION_RESULT_H


#ifndef F_CHECK
    #define F_CHECK(_expr, _res_ptr, _val, _ret)    \
        if(!(_expr)) {                              \
            if(_res_ptr) *_res_ptr = _val;          \
                                                    \
            return _ret;                            \
        }
#endif // F_CHECK


typedef enum {
    F_SUCCESS,
    F_ERR_PARAMS,
    F_ERR_BOUNDS,
    F_ERR_MEMORY,
    F_ERR_INTERNAL,
    F_ERR_NO_SUPPORTED_GPU
} f_res;


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
const char* f_res_to_str(f_res _res);
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // FACTION_RESULT_H
