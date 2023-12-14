#ifndef FACTION_WINDOW_H
#define FACTION_WINDOW_H


#include "result.h"


typedef struct f_window f_window;


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
f_res f_create_window(f_window** _win, int _w, int _h, const char* _title);
void f_destroy_window(f_window* _win);
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // FACTION_WINDOW_H
