#include "result.h"


const char* f_res_to_str(f_res _res) {
    switch (_res) {
        case F_SUCCESS:         return "Success";
        case F_ERR_PARAMS:      return "Invalid parameter(s)";
        case F_ERR_BOUNDS:      return "Value out of bounds";
        case F_ERR_MEMORY:      return "Insufficient memory";
        case F_ERR_INTERNAL:    return "Unsuccessful internal operation";
        default: break;
    }

    return "Unknown error";
}
