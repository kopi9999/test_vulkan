#ifndef VULKAN_INIT
#define VULKAN_INIT

#include <stdio.h>

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

extern VkInstance instance;
extern VkInstanceCreateInfo createInfo;
extern VkApplicationInfo appInfo;

void createVulkanInstance();
char checkValidationLayerSupport();
void initVulkan();

#endif

//#define DEB