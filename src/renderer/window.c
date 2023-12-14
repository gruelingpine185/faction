#include <GLFW/glfw3.h>

#include "renderer/window.h"


struct f_window {
    GLFWwindow* handle;
    const char* title;
    int w;
    int h;
};
