#include "window.h"

GLFWwindow* window;

void initWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(INIT_WIDTH, INIT_HEIGHT, WINDOW_NAME, NULL, NULL);
}
