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
    F_ERR_INTERNAL
} f_res;

#endif // FACTION_RESULT_H
