#ifndef FACTION_WINDOW_H
#define FACTION_WINDOW_H


#include "result.h"


typedef struct f_window f_window;


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
f_res f_create_window(f_window** _win, int _w, int _h, const char* _title);
void f_destroy_window(f_window* _win);
void f_window_poll_events(void);
int f_is_window_open(f_window* _win);
const char* f_get_window_title(const f_window* _win, f_res* _res);
int f_get_window_width(const f_window* _win, f_res* _res);
int f_get_window_height(const f_window* _win, f_res* _res);
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // FACTION_WINDOW_H
