//
// Created by jorge on 05/09/2023.
//
#pragma once

#include <vulkan/vulkan_raii.hpp>

#include "render_pass.hpp"
#include "swap_chain.hpp"

namespace vkengine
{
class FrameBuffers
{
public:
    explicit FrameBuffers(const vk::raii::Device& device, const SwapChain& swapChain, const RenderPass& renderPass);

    std::vector<vk::raii::Framebuffer> swapChainFramebuffers;
};
}
