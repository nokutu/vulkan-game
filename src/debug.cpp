#ifndef NDEBUG

#include "debug.hpp"

#include <iostream>

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
  VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
  VkDebugUtilsMessageTypeFlagsEXT messageType,
  const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
  void* pUserData)
{
    if (messageSeverity > VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
    {
        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
    }

    return VK_FALSE;
}

void DebugMessenger::populateDebugMessengerCreateInfo(vk::DebugUtilsMessengerCreateInfoEXT& createInfo)
{
    createInfo.sType = vk::StructureType::eDebugUtilsMessengerCreateInfoEXT;
    createInfo.messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
                                 vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                                 vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;
    createInfo.messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
                             vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
                             vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation;
    createInfo.pfnUserCallback = debugCallback;
}

void DebugMessenger::setupDebugMessenger(const vk::Instance& instance)
{
    vk::DispatchLoaderDynamic dispatcher(instance, vkGetInstanceProcAddr);

    vk::DebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(createInfo);
    debugMessenger = instance.createDebugUtilsMessengerEXT(createInfo, nullptr, dispatcher);
}
void DebugMessenger::destroy(const vk::Instance& instance, const vk::AllocationCallbacks* pAllocator)
{
    vk::DispatchLoaderDynamic dispatcher(instance, vkGetInstanceProcAddr);
    instance.destroyDebugUtilsMessengerEXT(debugMessenger, pAllocator, dispatcher);
}

#endif
