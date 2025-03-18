#include "vinit.h"
#include "pipeline.h"

VkShaderModule createShaderModule(const uint32_t* code, uint32_t codeLength)
{
    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = codeLength;
    createInfo.pCode = code;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    
    VkShaderModule shaderModule;
    if (vkCreateShaderModule(logicalDevice, &createInfo, NULL, &shaderModule) != VK_SUCCESS) {
        printf("ERROR: Failed to create shader module!\n");
        exit(0);
    }
    return shaderModule;
}

uint8_t readFile (void * buffer, size_t* length, const char* name, const char* method)
{
    if(name == NULL || length == NULL || method == NULL){return 1;} // code 1 - bad arguments
    FILE* file = fopen(name, method);
    if(file == NULL){return 2;} // code 2 - could not open file
    
    fseek(file, 0, SEEK_END);
    *length = ftell(file);
    rewind(file);
    
    if(ferror(file)){fclose(file); return 3;}// code 3 - could not read file
    if(buffer == NULL){fclose(file); return 0;}

    fread(buffer, *length, 1, file);
    
    if(ferror(file) || feof(file)){fclose(file); return 3;}// code 3 - could not read file
    fclose(file);  
    return 0;
}

void createGraphicsPipeline()
{
    uint8_t error;
    size_t vertShaderLength;
    readFile(NULL, &vertShaderLength, "./build/shaders/vert.spv", "rb");
    uint32_t vertShader[vertShaderLength];

    error = readFile(vertShader, &vertShaderLength, "./build/shaders/vert.spv", "rb");
    if(error){printf("ERROR: could not read vertex shader\n");}
    
    size_t fragShaderLength;
    readFile(NULL, &fragShaderLength, "./build/shaders/frag.spv", "rb");
    uint32_t fragShader[fragShaderLength];

    error = readFile(fragShader, &fragShaderLength, "./build/shaders/frag.spv", "rb");
    if(error){printf("ERROR: could not read fragment shader\n");}
    
    VkShaderModule vertShaderModule = createShaderModule(vertShader, vertShaderLength);
    VkShaderModule fragShaderModule = createShaderModule(fragShader, fragShaderLength);
    printf("Info: successfully created shader modules\n");

    vkDestroyShaderModule(logicalDevice, vertShaderModule, NULL);
    vkDestroyShaderModule(logicalDevice, fragShaderModule, NULL);
}
