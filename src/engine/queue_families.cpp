//
// Created by jorge on 30/08/2023.
//

#pragma once

#include "queue_families.hpp"


vkengine::QueueFamilyIndices vkengine::findQueueFamilies(const vk::raii::PhysicalDevice& device, const Window& window)
{
    QueueFamilyIndices indices;

    std::vector<vk::QueueFamilyProperties> queueFamilies = device.getQueueFamilyProperties();

    int i = 0;
    for (const auto& queueFamily : queueFamilies)
    {
        if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
        {
            indices.graphicsFamily = i;
        }

        vk::Bool32 presentSupport = presentSupport = device.getSurfaceSupportKHR(i, *window.surface);

        if (presentSupport)
        {
            indices.presentFamily = i;
        }

        if (indices.isComplete())
        {
            break;
        }

        i++;
    }

    return indices;
}
