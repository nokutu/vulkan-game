//
// Created by jorge on 30/08/2023.
//
#pragma once

#include <vulkan/vulkan_raii.hpp>

#include "render_pass.hpp"
#include "swap_chain.hpp"

namespace vkengine
{
class GraphicsPipeline
{
public:
    explicit GraphicsPipeline(const vk::raii::Device& device, const SwapChain& swapChain, const RenderPass& renderPass);

    vk::raii::PipelineLayout pipelineLayout = nullptr;
    vk::raii::Pipeline graphicsPipeline = nullptr;

    vk::raii::Pipeline& operator *();
};
}
