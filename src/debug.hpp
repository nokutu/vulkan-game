//
// Created by jorge on 8/8/2021.
//
#pragma once

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>

class DebugMessenger
{
public:
    static void populateDebugMessengerCreateInfo(vk::DebugUtilsMessengerCreateInfoEXT& createInfo);
    void setupDebugMessenger(const vk::Instance& instance);
    void destroy(const vk::Instance& instance, const vk::AllocationCallbacks* pAllocator);

private:
    vk::DebugUtilsMessengerEXT debugMessenger;
};
