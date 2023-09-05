//
// Created by jorge on 30/08/2023.
//
#pragma once

#include <vector>

#include <vulkan/vulkan_raii.hpp>

#include "engine/frame_buffers.hpp"
#include "engine/graphics_pipeline.hpp"
#include "engine/logical_device.hpp"
#include "engine/render_pass.hpp"
#include "engine/swap_chain.hpp"
#include "object.hpp"
#include "window.hpp"

namespace vkengine
{
class Renderer
{
public:
    explicit Renderer(
      const LogicalDevice& logicalDevice,
      const vk::raii::PhysicalDevice& physicalDevice,
      const Window& window,
      const SwapChain& swapChain,
      const RenderPass& renderPass,
      const GraphicsPipeline& graphicsPipeline,
      const FrameBuffers& frameBuffers);

    void add_object(const std::vector<Vertex>& vertices);

    void drawLoop();

private:
    const LogicalDevice& logicalDevice;
    const vk::raii::PhysicalDevice& physicalDevice;
    const Window& window;
    const SwapChain& swapChain;
    const RenderPass& renderPass;
    const GraphicsPipeline& graphicsPipeline;
    const FrameBuffers& frameBuffers;

    std::vector<Object> objects;
    vk::raii::CommandPool commandPool = nullptr;
    std::vector<vk::raii::CommandBuffer> commandBuffers;

    std::vector<vk::raii::Semaphore> imageAvailableSemaphores;
    std::vector<vk::raii::Semaphore> renderFinishedSemaphores;
    std::vector<vk::raii::Fence> inFlightFences;
    std::vector<vk::raii::Fence> imagesInFlight;
    size_t currentFrame = 0;
    bool initialized = false;
    bool valid = false;

    void createSyncObjects();
    void drawFrame();
    void refresh_command_buffers();
};
}
