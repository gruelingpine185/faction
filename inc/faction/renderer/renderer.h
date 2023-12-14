#ifndef FACTION_RENDERER_H
#define FACTION_RENDERER_H


#include "result.h"


typedef struct f_renderer f_renderer;

typedef struct f_window f_window;


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
f_res f_create_renderer(f_renderer** _renderer, f_window* _win);
void f_destroy_renderer(f_renderer* _renderer);
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // FACTION_RENDERER_H
