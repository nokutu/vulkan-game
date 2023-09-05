//
// Created by jorge on 30/08/2023.
//
#pragma once

#include "../window.hpp"
#include <vulkan/vulkan_raii.hpp>

namespace vkengine
{
struct SwapChainSupportDetails
{
    vk::SurfaceCapabilitiesKHR capabilities;
    std::vector<vk::SurfaceFormatKHR> formats;
    std::vector<vk::PresentModeKHR> presentModes;
};

SwapChainSupportDetails querySwapChainSupport(const vk::raii::PhysicalDevice& device, const Window& window);

class SwapChain
{
public:
    explicit SwapChain(
      const vk::raii::Device& device,
      const vk::raii::PhysicalDevice& physicalDevice,
      const Window& window);

    vk::raii::SwapchainKHR swapChain = nullptr;
    std::vector<VkImage> swapChainImages;
    vk::Format swapChainImageFormat;
    vk::Extent2D swapChainExtent;
    std::vector<vk::raii::ImageView> swapChainImageViews;

    vk::raii::SwapchainKHR& operator*();

private:
    void createImageViews(const vk::raii::Device& device);
};
}
