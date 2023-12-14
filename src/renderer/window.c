#include <stddef.h>

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
