#include "vinit.h"
#include "misc.h"
#include "window.h"
#include "misc.h"
#include <limits.h>

#ifdef DEB
	const char* validationLayers[1] = {"VK_LAYER_KHRONOS_validation"};
	const uint32_t validationLayerCount = 1;
	const char* validationExtensions[2] = {VK_EXT_DEBUG_UTILS_EXTENSION_NAME, VK_EXT_DEBUG_REPORT_EXTENSION_NAME};
	const uint32_t validationExtensionCount = 2;
#else
    const char* validationLayers[0];
	const uint32_t validationLayerCount = 0;
	const char* validationExtensions[0];
	const uint32_t validationExtensionCount = 0;
#endif

const char* requiredDeviceExtensions[1] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
uint32_t requiredDeviceExtensionCount = 1;

VkInstance instance;
VkInstanceCreateInfo createInfo;
VkApplicationInfo appInfo;
VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
VkDevice logicalDevice = VK_NULL_HANDLE;
VkQueue graphicsQueue;
VkQueue presentationQueue;
VkDebugUtilsMessengerEXT debugMessenger;
VkSurfaceKHR surface;
VkSwapchainKHR swapChain;
uint32_t swapChainImageCount;
VkFormat swapChainImageFormat;
VkExtent2D swapChainExtent;


//debug messenger callback:
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) 
{

    printf("validation layer: %s\n", pCallbackData->pMessage);

    return VK_FALSE;
}

void populateDebugUtilsMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT* createInfo)
{
	createInfo->sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo->messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo->messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo->pfnUserCallback = debugCallback;
	createInfo->pUserData = NULL;
	createInfo->pNext = NULL;
}


//instance creation:
void createVulkanInstance() 
{
	if(enableValidationLayers && !checkValidationLayerSupport()){
		printf("Warning: Validation layers requested, but not detected. Disableing now\n");
		createInfo.enabledLayerCount = 0;
	}
	else{
		createInfo.enabledLayerCount = validationLayerCount;
		createInfo.ppEnabledLayerNames = validationLayers;
		printf("Info: Validation layers enabled\n");
	}

    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = APPLICATION_NAME;
    appInfo.applicationVersion = APPLICATION_VERSION;
    appInfo.pEngineName = ENGINE_NAME;
    appInfo.engineVersion = ENGINE_VERSION;
    appInfo.apiVersion = API_VERSION;
    
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	
	if(enableValidationLayers){
		glfwExtensionCount += validationExtensionCount;
	}

	const char* extensions[glfwExtensionCount];

	if(enableValidationLayers){
		for (uint32_t i = 0; i < validationExtensionCount; i++){
			extensions[glfwExtensionCount - validationExtensionCount + i] = validationExtensions[i];
		}
	}

	for (uint32_t i = 0; i < glfwExtensionCount - validationExtensionCount; i++){
		extensions[i] = glfwExtensions[i];
	}

	createInfo.enabledExtensionCount = glfwExtensionCount;
	createInfo.ppEnabledExtensionNames = extensions;

	if(enableValidationLayers){
		printf("Available glfw extensions: %d\nExtension names: ", glfwExtensionCount);
		for(uint32_t i = 0; i < glfwExtensionCount; i++){
			printf("%s\n", extensions[i]);
		}
		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
		populateDebugUtilsMessengerCreateInfo(&debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
	}
	else{
		createInfo.pNext = NULL;
	}

	VkResult result = vkCreateInstance(&createInfo, NULL, &instance);
	
	if (result != VK_SUCCESS) {
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

//surface creation:
void createSurface(){
	
	if(glfwCreateWindowSurface(instance, window, NULL, &surface) != VK_SUCCESS){
		printf("ERROR: Cannot create window surface!");
	}
	else{printf("Info: successfully created window surface\n");}
}

// physical device obtain:
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

char checkDeviceExtensionSupport(VkPhysicalDevice device)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, NULL, &extensionCount, NULL);
	VkExtensionProperties extensions[extensionCount];
	vkEnumerateDeviceExtensionProperties(device, NULL, &extensionCount, extensions);
	
	char isSuitable = 1;
	char support;
	for(uint32_t i = 0; i<requiredDeviceExtensionCount; i++){
		support = 0;
		for(uint32_t j = 0; j<extensionCount; j++){
			if(strcmpr(requiredDeviceExtensions[i], extensions[j].extensionName)){
				support = 1;
				break;
			}
		}
		if(!support){isSuitable = 0; break;}
	}

	if (isSuitable){return 1;}
	return 0;
}

char isDeviceSuitable(VkPhysicalDevice device) 
{
	struct QueueFamilyIndices indicies;

	if(findQueueFamilies(device, &indicies) && checkDeviceExtensionSupport(device)){return 1;}
	return 0;

    /*VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;

    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
    if(deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader){return 1;}
    return 0;*/
}

//logical device creation and queue finding:
char findQueueFamilies(VkPhysicalDevice device, struct QueueFamilyIndices* indices)
{
	indices->isUsed = 0;
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, NULL);

	VkQueueFamilyProperties queueFamilies[queueFamilyCount];
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies);

	VkBool32 presentationSupport;
	for(uint32_t i = 0; i < queueFamilyCount; i++){
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentationSupport);
		if(queueFamilies[i].queueFlags && VK_QUEUE_GRAPHICS_BIT && indices->isUsed != 1){
			indices->graphicsFamily = i;
			indices->isUsed += 1;
		}
		if (presentationSupport && indices->isUsed != 2)
		{
			indices->presentationFamily = i;
			indices->isUsed += 2;
		}
		if(indices->isUsed == 3){break;}
	}

	if(indices->isUsed == 3){return 1;}
	return 0;
}

void createLogicalDevice()
{
	struct QueueFamilyIndices indices;
	if(findQueueFamilies(physicalDevice, &indices)){
		printf("Info: found supported queue families - %d, %d\n", indices.graphicsFamily, indices.presentationFamily);
	}
	else{
		printf("ERROR: cannot find supported queue families\n");
		exit(1);
	}

	char uniqueQueueFamilyCount;
	if(indices.graphicsFamily == indices.presentationFamily){uniqueQueueFamilyCount = 1;}
	else{uniqueQueueFamilyCount = 2;}
	uint32_t uniqueQueueFamilies[uniqueQueueFamilyCount];
	VkDeviceQueueCreateInfo queueCreateInfo[uniqueQueueFamilyCount];
	uniqueQueueFamilies[0] = indices.graphicsFamily;
	if(uniqueQueueFamilyCount-1){uniqueQueueFamilies[1] = indices.presentationFamily;}

	float queuePriority = 1.0;
	for (char i = 0; i < uniqueQueueFamilyCount; i++){
		queueCreateInfo[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo[i].queueFamilyIndex = uniqueQueueFamilies[i];
		queueCreateInfo[i].queueCount = 1;
		queueCreateInfo[i].pQueuePriorities = &queuePriority;
		queueCreateInfo[i].flags = NULL;
		queueCreateInfo[i].pNext = NULL;
	}

	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);

	VkDeviceCreateInfo deviceCreateInfo;
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.pQueueCreateInfos = queueCreateInfo;
	deviceCreateInfo.queueCreateInfoCount = uniqueQueueFamilyCount;
	deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

	deviceCreateInfo.enabledExtensionCount = requiredDeviceExtensionCount;
	deviceCreateInfo.ppEnabledExtensionNames = requiredDeviceExtensions;

	if (enableValidationLayers) {
    	deviceCreateInfo.enabledLayerCount = validationLayerCount;
    	deviceCreateInfo.ppEnabledLayerNames = validationLayers;

	} else {
    	deviceCreateInfo.enabledLayerCount = 0;
	}
	deviceCreateInfo.flags = NULL;
	deviceCreateInfo.pNext = NULL;

	VkResult result = vkCreateDevice(physicalDevice, &deviceCreateInfo, NULL, &logicalDevice);

	if (result != VK_SUCCESS) {
    	printf("ERROR: failed to create logical device, code: %d\n", result);
		exit(1);
	}
	printf("Info: succesfully created logical device\n");
	
	vkGetDeviceQueue(logicalDevice, indices.graphicsFamily, 0, &graphicsQueue);
	vkGetDeviceQueue(logicalDevice, indices.presentationFamily, 0, &presentationQueue);
}

//swap chain creation:
VkSurfaceFormatKHR chooseSwapSurfaceFormat(const VkSurfaceFormatKHR* availableFormats, const uint32_t availableFormatCount)
{
	for(uint32_t i = 0; i < availableFormatCount; i++){
		if(availableFormats[i].format == VK_FORMAT_B8G8R8A8_SRGB 
			&& availableFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR){return availableFormats[i];}
	}
	printf("Info: Could not find prefered color space, or surface format. Using default");
	return availableFormats[0];
}

VkPresentModeKHR chooseSwapPresentMode(const VkPresentModeKHR* availablePresentModes, const uint32_t availablePresentModeCount) {
    for (uint32_t i = 0; i < availablePresentModeCount; i++){
		if(availablePresentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR){return availablePresentModes[i];}
	}
	printf("Info: Could not find tripple buffered presentation mode, using dubble buffered instead");
	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR* capabilities)
{
	if (capabilities->currentExtent.width != UINT32_MAX) {
		return capabilities->currentExtent;
    } else {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        VkExtent2D actualExtent = {
            (uint32_t) width,
            (uint32_t) height
        };

		if(actualExtent.width < capabilities->minImageExtent.width){actualExtent.width = capabilities->minImageExtent.width;}
		else if(actualExtent.width > capabilities->maxImageExtent.width){actualExtent.width = capabilities->maxImageExtent.width;}
		
		if(actualExtent.height < capabilities->minImageExtent.height){actualExtent.height = capabilities->minImageExtent.height;}
		else if(actualExtent.height > capabilities->maxImageExtent.height){actualExtent.height = capabilities->maxImageExtent.height;}

        return actualExtent;
    }
}

void createSwapChain()
{
	VkSurfaceCapabilitiesKHR capabilities;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, NULL);
	if (formatCount == 0) {printf("ERROR: cannot find any device surface format");}
	VkSurfaceFormatKHR formats[formatCount];
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, formats);

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, NULL);
	if (presentModeCount == 0) {printf("ERROR: cannot find any device surface format");}
	VkPresentModeKHR presentModes[formatCount];
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &formatCount, presentModes);

	VkSurfaceFormatKHR usedFormat = chooseSwapSurfaceFormat(formats, formatCount);
	VkPresentModeKHR usedPresentMode = chooseSwapPresentMode(presentModes, presentModeCount);
    VkExtent2D extent = chooseSwapExtent(&capabilities);

	swapChainImageCount = capabilities.minImageCount + 1;
	if (capabilities.maxImageCount > 0 && swapChainImageCount > capabilities.maxImageCount) {swapChainImageCount = capabilities.maxImageCount;}
	
	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface;
	createInfo.minImageCount = swapChainImageCount;
	createInfo.imageFormat = usedFormat.format;
	createInfo.imageColorSpace = usedFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	createInfo.preTransform = capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = usedPresentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;


	struct QueueFamilyIndices indices = {};
	findQueueFamilies(physicalDevice, &indices);
	uint32_t queueFamilyIndices[2];
	queueFamilyIndices[0] = indices.graphicsFamily;
	queueFamilyIndices[1] = indices.presentationFamily;


	if (indices.graphicsFamily != indices.presentationFamily) {
    	createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    	createInfo.queueFamilyIndexCount = 2;
    	createInfo.pQueueFamilyIndices = queueFamilyIndices;
	} else {
    	createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    	createInfo.queueFamilyIndexCount = 0;
    	createInfo.pQueueFamilyIndices = NULL;
	}

	if (vkCreateSwapchainKHR(logicalDevice, &createInfo, NULL, &swapChain) != VK_SUCCESS) {
    	printf("ERROR: failed to create swap chain!\n");
	}
	else {printf("Info: successfully created swap chain\n");}

	swapChainImageFormat = usedFormat.format;
	swapChainExtent = extent;
}

//debug messenger setup:
void setupDebugMessenger() 
{
    if (!enableValidationLayers) return;

	VkDebugUtilsMessengerCreateInfoEXT createInfo;
	populateDebugUtilsMessengerCreateInfo(&createInfo);

	VkResult result = CreateDebugUtilsMessengerEXT(instance, &createInfo, NULL, &debugMessenger);

	if (result != VK_SUCCESS) {
    	printf("ERROR: failed to set up debug messenger!: %d\n", result);
	}

}

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, 
const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
const VkAllocationCallbacks* pAllocator, 
VkDebugUtilsMessengerEXT* pDebugMessenger) 
{
    
	VkResult (*func)(VkInstance, const VkDebugUtilsMessengerCreateInfoEXT*, const VkAllocationCallbacks*,VkDebugUtilsMessengerEXT*) = 
		(PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    
	if (func != NULL) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
	else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, 
VkDebugUtilsMessengerEXT debugMessenger, 
const VkAllocationCallbacks* pAllocator) 
{
    void (*func)(VkInstance, VkDebugUtilsMessengerEXT, const VkAllocationCallbacks*) = 
		(PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    
	if (func != NULL) {
        (*func)(instance, debugMessenger, pAllocator);
    }
}


void initVulkan()
{
	createVulkanInstance();
	setupDebugMessenger();

	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, NULL);
	VkExtensionProperties extensions[extensionCount];
	vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, extensions);
	
	printf("Available extensions: %d\nExtension names: ", extensionCount);
	for(char i = 0; i < extensionCount; i++){
		printf("%s\n", extensions[i].extensionName);
	}

	createSurface();

    pickPhysicalDevice();
	createLogicalDevice();

	createSwapChain();
}

