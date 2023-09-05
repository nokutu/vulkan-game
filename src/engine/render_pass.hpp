//
// Created by jorge on 30/08/2023.
//

#pragma once

#include <vulkan/vulkan_raii.hpp>

#include "swap_chain.hpp"

namespace vkengine
{
class RenderPass
{
public:
    explicit RenderPass(const vk::raii::Device& device, const SwapChain& swapChain);

    vk::raii::RenderPass renderPass = nullptr;

    vk::raii::RenderPass& operator *();
};
}
