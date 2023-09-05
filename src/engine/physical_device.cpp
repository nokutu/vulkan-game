//
// Created by jorge on 30/08/2023.
//

#include "physical_device.hpp"

#include <set>

#include "queue_families.hpp"
#include "swap_chain.hpp"

bool checkDeviceExtensionSupport(const vk::raii::PhysicalDevice& device)
{
    std::vector<vk::ExtensionProperties> availableExtensions = device.enumerateDeviceExtensionProperties();

    std::set<std::string> requiredExtensions(
      vkengine::DEVICE_EXTENSIONS.begin(), vkengine::DEVICE_EXTENSIONS.end());

    for (const auto& extension : availableExtensions)
    {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

bool isDeviceSuitable(const vk::raii::PhysicalDevice& device, const Window& window)
{
    vkengine::QueueFamilyIndices indices = vkengine::findQueueFamilies(device, window);

    bool extensionsSupported = checkDeviceExtensionSupport(device);

    bool swapChainAdequate = false;
    if (extensionsSupported)
    {
        vkengine::SwapChainSupportDetails swapChainSupport = vkengine::querySwapChainSupport(device, window);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    return indices.isComplete() && extensionsSupported && swapChainAdequate;
}

vkengine::PhysicalDevice::PhysicalDevice(const vk::raii::Instance& instance, const Window& window)
{
    for (auto&& candidateDevice : instance.enumeratePhysicalDevices())
    {
        if (isDeviceSuitable(candidateDevice, window))
        {
            physicalDevice = std::move(candidateDevice);
            return;
        }
    }

    throw std::runtime_error("failed to find a suitable GPU!");
}

vk::raii::PhysicalDevice& vkengine::PhysicalDevice::operator*()
{
    return physicalDevice;
}
