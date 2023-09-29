#include <stdio.h>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

GLFWwindow* window;
VkInstance instance;
VkInstanceCreateInfo createInfo;
VkApplicationInfo appInfo;

void initWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(800, 600, "Vulkan", NULL, NULL);
}

void createVulkanInstance() {
    	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    	appInfo.pApplicationName = "Hello Triangle";
    	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    	appInfo.pEngineName = "No Engine";
    	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    	appInfo.apiVersion = VK_API_VERSION_1_0;
    
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	createInfo.enabledExtensionCount = glfwExtensionCount;
	createInfo.ppEnabledExtensionNames = glfwExtensions;
	
	VkResult result = vkCreateInstance(&createInfo, NULL, &instance);
	
	if (vkCreateInstance(&createInfo, NULL, &instance) != VK_SUCCESS) {
    		printf("failed to create instance!");
	}
}

void initVulkan()
{
	createVulkanInstance();
	
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
	initVulkan();
	mainLoop();
	cleanup();
	printf("yo");
	return 0;
}
