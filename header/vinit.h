#ifndef VULKAN_INIT
#define VULKAN_INIT

#include <stdio.h>
#include <stdlib.h>

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

struct QueueFamilyIndices {
    uint32_t graphicsFamily;
    char isUsed;
};

extern VkInstance instance;
extern VkInstanceCreateInfo createInfo;
extern VkApplicationInfo appInfo;
extern VkDevice logicalDevice;
extern VkQueue graphicsQueue;

//void createVulkanInstance();
char checkValidationLayerSupport();
void initVulkan();
char isDeviceSuitable(VkPhysicalDevice);
char findQueueFamilies(VkPhysicalDevice, struct QueueFamilyIndices*);

#endif

//#define DEB