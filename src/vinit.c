#include "vinit.h"
#include "misc.h"

#ifdef DEB
	const char enableValidationLayers = 1;
	const char* validationLayers[1] = {"VK_LAYER_KHRONOS_validation"};
#else
	const char enableValidationLayers = 0;
    const char* validationLayers[0];
#endif

VkInstance instance;
VkInstanceCreateInfo createInfo;
VkApplicationInfo appInfo;
VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

void createVulkanInstance() 
{
	if(enableValidationLayers && !checkValidationLayerSupport()){
		printf("Warning: Validation layers requested, but not detected. Disableing now\n");
		createInfo.enabledLayerCount = 0;
	}
	else{
		createInfo.enabledLayerCount = sizeof(validationLayers)/sizeof(validationLayers[0]);
		createInfo.ppEnabledLayerNames = validationLayers;
		printf("Info: Validation layers enabled\n");
	}

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

void pickPhysicalDevice()
{
    int deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, NULL);
    if(deviceCount == 0){
        printf("ERROR: cannot find any graphics card with Vulkan support\n");
        exit(1);
    }

    VkPhysicalDevice physicalDevices[deviceCount];
    vkEnumeratePhysicalDevices(instance, &deviceCount, physicalDevices);

    for (int i = 0; i < deviceCount; i++) {
        if (isDeviceSuitable(physicalDevices[i])) {
            physicalDevice = physicalDevices[i];
            break;
        }
    }

    if (physicalDevice == VK_NULL_HANDLE) {
        printf("ERROR: no graphics cards are supported\n");
        exit(1);
    }
    else{printf("Info: Found a supported graphics card\n");}
}

char isDeviceSuitable(VkPhysicalDevice device) {
    /*VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;

    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
    if(deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader){return 1;}
    return 0;*/
}

char findQueueFamilies(VkPhysicalDevice device, struct QueueFamilyIndices* indices)
{
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, NULL);

	VkQueueFamilyProperties queueFamilies[queueFamilyCount];
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies);

	for(uint32_t i = 0; i < queueFamilyCount; i++){
		if(queueFamilies[i].queueFlags && VK_QUEUE_GRAPHICS_BIT){
			indices->graphicsFamily = i;
			indices->isUsed = 1;
			break;}
	}

	if(indices->isUsed){return 1;}
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

    pickPhysicalDevice();

	struct QueueFamilyIndices indices;
	indices.isUsed = 0;
	if(findQueueFamilies(physicalDevice, &indices)){
		printf("Info: found supported queue family - %d\n", indices.graphicsFamily);
	}
	else{
		printf("ERROR: cannot find a supported queue family");
		exit(1);
	}
}