//
// Created by jorge on 8/8/2021.
//
#pragma once

#include <vulkan/vulkan_core.h>

class DebugMessenger
{
public:
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    void setupDebugMessenger(VkInstance& instance);
    void destroy(VkInstance& instance, const VkAllocationCallbacks* pAllocator);

private:
    VkDebugUtilsMessengerEXT debugMessenger;
};
