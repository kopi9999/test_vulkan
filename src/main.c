#include <stdio.h>
#include <GLFW/glfw3.h>

GLFWwindow* window;

void initWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(800, 600, "Vulkan", NULL, NULL);
}

void initVulkan()
{

}

void mainLoop()
{
	while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    	}
}

void cleanup()
{
	glfwDestroyWindow(window);

	glfwTerminate();
}


int main()
{
	initWindow();
	mainLoop();
	cleanup();
	printf("yo");
	return 0;
}
