//
// Created by jorge on 30/08/2023.
//
#pragma once

#include <vulkan/vulkan_raii.hpp>

#include "../window.hpp"

namespace vkengine
{

constexpr std::array<const char*, 1> DEVICE_EXTENSIONS = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

class PhysicalDevice
{
public:
    explicit PhysicalDevice(const vk::raii::Instance& instance, const Window& window);

    vk::raii::PhysicalDevice physicalDevice = nullptr;

    vk::raii::PhysicalDevice& operator *();

};
}
