//
// Created by jorge on 30/08/2023.
//
#pragma once

#include <vulkan/vulkan_raii.hpp>

#include "physical_device.hpp"
#include "../window.hpp"

namespace vkengine {

const std::array<const char*, 1> VALIDATION_LAYERS = { "VK_LAYER_KHRONOS_validation" };

class LogicalDevice
{
public:
    explicit LogicalDevice(const vk::raii::Instance& instance, const Window& window, const PhysicalDevice& physicalDevice);

    vk::raii::Device logicalDevice = nullptr;
    vk::raii::Queue graphicsQueue = nullptr;
    vk::raii::Queue presentQueue = nullptr;

    vk::raii::Device& operator *();

};
}

