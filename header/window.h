#ifndef WINDOW
#define WINDOW

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

extern GLFWwindow* window;

void initWindow();

#define INIT_WIDTH 800
#define INIT_HEIGHT 600
#define WINDOW_NAME "Vulkan test program"

#endif