//
// Created by jorge on 30/08/2023.
//

#include "logical_device.hpp"

#include <set>

#include "queue_families.hpp"

vkengine::LogicalDevice::LogicalDevice(
  const vk::raii::Instance& instance,
  const Window& window,
  const PhysicalDevice& physicalDevice)
{
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice.physicalDevice, window);

    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(),
                                               indices.presentFamily.value() };

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies)
    {
        vk::DeviceQueueCreateInfo queueCreateInfo{};
        // queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    vk::PhysicalDeviceFeatures deviceFeatures{};

    vk::DeviceCreateInfo createInfo{};
    // createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = static_cast<uint32_t>(DEVICE_EXTENSIONS.size());
    createInfo.ppEnabledExtensionNames = DEVICE_EXTENSIONS.data();

#ifndef NDEBUG
    createInfo.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
    createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
#else
    createInfo.enabledLayerCount = 0;
#endif

    logicalDevice = physicalDevice.physicalDevice.createDevice(createInfo);
    graphicsQueue = logicalDevice.getQueue(indices.graphicsFamily.value(), 0);
    presentQueue = logicalDevice.getQueue(indices.presentFamily.value(), 0);
}
vk::raii::Device& vkengine::LogicalDevice::operator*()
{
    return logicalDevice;
}
