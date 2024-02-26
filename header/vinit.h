#ifndef VULKAN_INIT
#define VULKAN_INIT
#define DEB

#include <stdio.h>
#include <stdlib.h>

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

struct QueueFamilyIndices {
    uint32_t graphicsFamily;
    char isUsed;
};

extern const char enableValidationLayers;

extern VkInstance instance;
extern VkInstanceCreateInfo createInfo;
extern VkApplicationInfo appInfo;
extern VkDevice logicalDevice;
extern VkQueue graphicsQueue;
extern VkDebugUtilsMessengerEXT debugMessenger;

char checkValidationLayerSupport();
void initVulkan();
char isDeviceSuitable(VkPhysicalDevice);
char findQueueFamilies(VkPhysicalDevice, struct QueueFamilyIndices*);
void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

#define APPLICATION_NAME "Vulkan test program"
#define APPLICATION_VERSION VK_MAKE_VERSION(1, 0, 0)
#define ENGINE_NAME "Vulkan test engine"
#define ENGINE_VERSION VK_MAKE_VERSION(1, 0, 0)
#define API_VERSION VK_API_VERSION_1_0

#endif