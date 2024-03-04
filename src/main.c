#include "window.h"
#include "vinit.h"

#ifdef DEB
	const char enableValidationLayers = 1;
#else
	const char enableValidationLayers = 0;
#endif

void mainLoop()
{
	while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    	}
}

void cleanup()
{
	if(enableValidationLayers){
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, NULL);
	}
    vkDestroySwapchainKHR(logicalDevice, swapChain, NULL);
	vkDestroySurfaceKHR(instance, surface, NULL);
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