#include "window.h"
#include "vinit.h"

void mainLoop()
{
	while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    	}
}

void cleanup()
{
	vkDestroyDevice(logicalDevice, NULL);
	vkDestroyInstance(instance, NULL);

	glfwDestroyWindow(window);

	glfwTerminate();
}


int main()
{
	initWindow();
	initVulkan();
	mainLoop();
	cleanup();
	printf("yo");
	return 0;
}
