#ifndef WINDOW
#define WINDOW

#include <GLFW/glfw3.h>

extern GLFWwindow* window;

void initWindow();

#define INIT_WIDTH 800
#define INIT_HEIGHT 600
#define WINDOW_NAME "Vulkan test program"

#endif