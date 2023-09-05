//
// Created by jorge on 30/08/2023.
//

#pragma once

#include <optional>

#include <vulkan/vulkan_raii.hpp>

#include "../window.hpp"

namespace vkengine
{

struct QueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    [[nodiscard]] bool isComplete() const { return graphicsFamily.has_value() && presentFamily.has_value(); }
};

QueueFamilyIndices findQueueFamilies(const vk::raii::PhysicalDevice& device, const Window& window);
}
