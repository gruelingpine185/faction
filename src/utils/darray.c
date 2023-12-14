#include "utils/darray.h"


struct f_darray {
    void** data;
    size_t cap;
    size_t size;
};
