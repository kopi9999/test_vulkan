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




//void callbackLogger(void* value) 
//{
//    printf("%d\n", (uint64_t)value);
//    return;
//}


//void* allocationt_works() {
//    Callback(void* pUserData, 
//                         size_t size, 
//                         size_t allignment, 
//                         VkSystemAllocationScope allocationScope)
//{
//    
//}
//
//
//VkAllocationCallbacks AllocationCallbacksSetup() 
//{
//    VkAllocationCallbacks callbacks = {};
//    callbacks.pUserData = callbackLogger;
//    callbacks.pfnAllocation = NULL;
//    callbacks.pfnReallocation = NULL;
//    callbacks.pfnFree = NULL;
//    callbacks.pfnInternalAllocation = NULL;
//    callbacks.pfnInternalFree = NULL;
//
//    return callbacks;
//}
//
//
//







//uint8_t readFile (void * buffer, size_t* length, const char* name, const char* method)
//{
//    if(name == NULL || length == NULL || method == NULL){return 1;} // code 1 - bad arguments
//    FILE* file = fopen(name, method);
//    if(file == NULL){return 2;} // code 2 - could not open file
//    
//    fseek(file, 0, SEEK_END);
//    *length = ftell(file);
//    rewind(file);
//    
//    if(ferror(file)){fclose(file); return 3;}// code 3 - could not read file
//    if(buffer == NULL){fclose(file); return 0;}
//
//    fread(buffer, *length, 1, file);
//    
//    if(ferror(file) || feof(file)){fclose(file); return 3;}// code 3 - could not read file
//    fclose(file);  
//    return 0;
//}

void createGraphicsPipeline()
{
    FILE* vertShaderFile = fopen("build/shaders/vert.spv", "rb");
    if(vertShaderFile == NULL){
        printf("ERROR: could not load vertex shader file\n");
        exit(0);
    }

    printf("info: Successfully found shader files\n");
    
    printf("info: Loading vertex shader: ");
    fseek(vertShaderFile, 0, SEEK_END);
    uint32_t vertShaderLength = ftell(vertShaderFile);
    rewind(vertShaderFile);
    printf("(%dB)", vertShaderLength);
    
    uint32_t vertShader[vertShaderLength/4];

    fread(vertShader, 1, vertShaderLength, vertShaderFile);

    int error = ferror(vertShaderFile);
    if (error || feof(vertShaderFile)){
        printf("error reading vertex shader file, code %d", error);
        exit(0);
    }

    fclose(vertShaderFile);
    printf(" - done\n");
    
    FILE* fragShaderFile = fopen("build/shaders/frag.spv", "rb");
    if(fragShaderFile == NULL){
        printf("ERROR: could not load fragment shader file\n");
        exit(0);
    }

    printf("info: Loading fragment shader: ");
    fseek(fragShaderFile, 0, SEEK_END);
    uint32_t fragShaderLength = ftell(fragShaderFile);
    rewind(fragShaderFile);
    printf("(%dB)", fragShaderLength);
    uint32_t fragShader[fragShaderLength/4];

    fread(fragShader, 1, fragShaderLength, fragShaderFile);
    
    error = ferror(fragShaderFile);
    if (error || feof(fragShaderFile)){
        printf("error reading fragment shader file, code %d", error);
        exit(0);
    }

    fclose(fragShaderFile);
    printf(" - done\n");
   
    //size_t vertShaderLength;
    //readFile(NULL, &vertShaderLength, "./build/shaders/vert.spv", "rb");
    //uint8_t vertShader[vertShaderLength];

    //printf("%d\n", readFile(vertShader, &vertShaderLength, "./build/shaders/vert.spv", "rb"));
    
    //FILE* vertShaderFile = fopen("build/shaders/vert.spv", "rb");
    
    //uint8_t vertShader8;
   // for (uint32_t i = 0; i<vertShaderLength; i++){
  //      fread(&vertShader8, 1, 1, vertShaderFile);
 //       printf("odczytane: %d, sprawdzenie: %d\n", (uint8_t)vertShader[i], vertShader8);
//    }
//    uint32_t vertShader32;
//    rewind(vertShaderFile);
//    uint32_t vertShader2[vertShaderLength/4];
//    for (uint32_t i = 0; i<vertShaderLength/4; i++){
//        fread(&vertShader32, 4, 1, vertShaderFile);
//        printf("odczytane: %d, sprawdzenie: %d\n", *((uint32_t*)vertShader+i), vertShader32);
//        vertShader2[i] = *((uint32_t*)vertShader+i);
//    }
//
//    VkShaderModuleCreateInfo createInfo = {};
//    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
//    createInfo.codeSize = vertShaderLength;
//    createInfo.pCode = vertShader2;
//    createInfo.pNext = NULL;
//    createInfo.flags = 0;
//   
//    VkShaderModule vertShaderModule;
//    if (vkCreateShaderModule(logicalDevice, &createInfo, NULL, &vertShaderModule) != VK_SUCCESS) {
//        printf("ERROR: Failed to create shader module!\n");
//        exit(0);
    //}
    
    VkShaderModule vertShaderModule = createShaderModule(vertShader, vertShaderLength);
    VkShaderModule fragShaderModule = createShaderModule(fragShader, fragShaderLength);
    printf("info: successfully created shader modules\n");

    vkDestroyShaderModule(logicalDevice, vertShaderModule, NULL);
    vkDestroyShaderModule(logicalDevice, fragShaderModule, NULL);
}
