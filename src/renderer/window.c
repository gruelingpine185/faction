#include <stddef.h>
#include <stdlib.h>

#include <GLFW/glfw3.h>

#include "renderer/window.h"


struct f_window {
    GLFWwindow* handle;
    const char* title;
    int w;
    int h;
};


GLFWwindow* f_glfw_create_window(int* _win, int* _h, const char* _title);
void f_glfw_destroy_window(GLFWwindow* _win);


GLFWwindow* f_glfw_create_window(int* _w, int* _h, const char* _title) {
    if(glfwInit() != GLFW_TRUE) return NULL;

    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* win = glfwCreateWindow(*_w, *_h, _title, NULL, NULL);
    if(!win) {
        f_glfw_destroy_window(win);
        return NULL;
    }

    glfwGetWindowSize(win, _w, _h);
    return win;
}

void f_glfw_destroy_window(GLFWwindow* _w) {
    glfwDestroyWindow(_w);
    glfwTerminate();
}

f_res f_create_window(f_window** _win, int _w, int _h, const char* _title) {
    if(!_win || (_w < 1) || (_h < 1)) return F_ERR_PARAMS;

    f_window* win = (f_window*) malloc(sizeof(*win));
    if(!win) return F_ERR_MEMORY;

    win->title = (_title)? _title: "";
    win->w = _w;
    win->h = _h;
    win->handle = f_glfw_create_window(&win->w, &win->h, win->title);
    if(!win->handle) {
        f_destroy_window(win);
        return F_ERR_INTERNAL;
    }

    glfwShowWindow(win->handle);
    *_win = win;
    return F_SUCCESS;
}

void f_destroy_window(f_window* _win) {
    if(!_win) return;

    if(_win->handle) f_glfw_destroy_window(_win->handle);

    free(_win);
}

void f_window_poll_events(void) {
    glfwPollEvents();
}

int f_is_window_open(f_window* _win) {
    if(!_win) return 0;

    return (!glfwWindowShouldClose(_win->handle));
}

const char* f_get_window_title(const f_window* _win, f_res* _res) {
    if(!_win) {
        if(_res) *_res = F_ERR_PARAMS;

        return NULL;
    }

    if(_res) *_res = F_SUCCESS;

    return _win->title;
}

int f_get_window_width(const f_window* _win, f_res* _res) {
    if(!_win) {
        if(_res) *_res = F_ERR_PARAMS;

        return 0;
    }

    if(_res) *_res = F_SUCCESS;

    return _win->w;
}

int f_get_window_height(const f_window* _win, f_res* _res) {
    if(!_win) {
        if(_res) *_res = F_ERR_PARAMS;

        return 0;
    }

    if(_res) *_res = F_SUCCESS;

    return _win->h;
}
