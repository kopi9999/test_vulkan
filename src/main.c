#include <stdio.h>
#include <string.h>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#define DEB

#ifdef DEB
	char enableValidationLayers = 1;
	const char* validationLayers[1] = {"VK_LAYER_KHRONOS_validation"};
#else
	const char enableValidationLayers = 0;
#endif

GLFWwindow* window;
VkInstance instance;
VkInstanceCreateInfo createInfo;
VkApplicationInfo appInfo;

char checkValidationLayerSupport();

char strcmpr(const char* a, const char* b)
{
	int al = strlen(a);
	if(al != strlen(b)){return 0;}
	for(int i = 0; i < al; i++){
		if(a[i] != b[i]){return 0;}
	}
	return 1;
}


void initWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(800, 600, "Vulkan", NULL, NULL);
}

void createVulkanInstance() 
{
	if(enableValidationLayers && !checkValidationLayerSupport()){
		printf("Warning: Validation layers requested, but not detected. Disableing now\n");
		enableValidationLayers = 0;
	}
	else{printf("Info: Validation layers enabled\n");}

	//createInfo.enabledLayerCount = 

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

char checkValidationLayerSupport()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, NULL);

    VkLayerProperties availableLayers[layerCount];
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers);
	char layerFound = 0;
	for(int i = 0; i < sizeof(validationLayers)/sizeof(validationLayers[0]); i++){
		for (int j = 0; j < layerCount; j++) {
        	if (strcmpr(validationLayers[i], availableLayers[j].layerName)) {layerFound += 1; break;}
    	}
	}
	if(layerFound == sizeof(validationLayers)/sizeof(validationLayers[0])){return 1;}
	return 0;
}

void initVulkan()
{
	createVulkanInstance();

	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, NULL);
	VkExtensionProperties extensions[extensionCount];
	vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, extensions);
	
	printf("Available extensions: %d\nExtension names: ", extensionCount);
	for(char i = 0; i < extensionCount; i++){
		printf("%s\n", extensions[i].extensionName);
	}
}

void mainLoop()
{
	while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    	}
}

void cleanup()
{
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
