//
// Created by jorge on 30/08/2023.
//

#include "renderer.hpp"

#include <iostream>

#include "engine/queue_families.hpp"

constexpr int MAX_FRAMES_IN_FLIGHT = 2;

vkengine::Renderer::Renderer(
  const LogicalDevice& logicalDevice,
  const vk::raii::PhysicalDevice& physicalDevice,
  const Window& window,
  const vkengine::SwapChain& swapChain,
  const vkengine::RenderPass& renderPass,
  const vkengine::GraphicsPipeline& graphicsPipeline,
  const vkengine::FrameBuffers& frameBuffers)
  : logicalDevice(logicalDevice)
  , physicalDevice(physicalDevice)
  , window(window)
  , swapChain(swapChain)
  , renderPass(renderPass)
  , graphicsPipeline(graphicsPipeline)
  , frameBuffers(frameBuffers)
{
    QueueFamilyIndices queueFamilyIndices = vkengine::findQueueFamilies(physicalDevice, window);

    vk::CommandPoolCreateInfo poolInfo{};
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
    commandPool = logicalDevice.logicalDevice.createCommandPool(poolInfo);
    createSyncObjects();
}

void vkengine::Renderer::refresh_command_buffers()
{
    vk::CommandBufferAllocateInfo allocInfo{};
    allocInfo.commandPool = *commandPool;
    allocInfo.level = vk::CommandBufferLevel::ePrimary;
    allocInfo.commandBufferCount = frameBuffers.swapChainFramebuffers.size();

    commandBuffers = logicalDevice.logicalDevice.allocateCommandBuffers(allocInfo);

    for (size_t i = 0; i < commandBuffers.size(); i++)
    {
        vk::CommandBufferBeginInfo beginInfo{};
        if (initialized) [[likely]]
        {
            commandBuffers[i].reset();
        }
        commandBuffers[i].begin(beginInfo);

        vk::RenderPassBeginInfo renderPassInfo{};
        renderPassInfo.renderPass = *renderPass.renderPass;
        renderPassInfo.framebuffer = *frameBuffers.swapChainFramebuffers[i];
        renderPassInfo.renderArea.offset = vk::Offset2D{ 0, 0 };
        renderPassInfo.renderArea.extent = swapChain.swapChainExtent;

        vk::ClearValue clearColor = vk::ClearColorValue(std::array<float, 4>({ { 0.f, 0.f, 0.f, 1.f } }));
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        commandBuffers[i].beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);

        commandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, *graphicsPipeline.graphicsPipeline);

        for (const auto& object : objects)
        {
            object.bind(commandBuffers[i]);
        }

        commandBuffers[i].endRenderPass();
        commandBuffers[i].end();
    }
    initialized = true;
    valid = true;
}

void vkengine::Renderer::createSyncObjects()
{
    imageAvailableSemaphores.reserve(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.reserve(MAX_FRAMES_IN_FLIGHT);
    inFlightFences.reserve(MAX_FRAMES_IN_FLIGHT);
    imagesInFlight.reserve(swapChain.swapChainImages.size());

    vk::SemaphoreCreateInfo semaphoreInfo{};
    vk::FenceCreateInfo fenceInfo{ vk::FenceCreateFlagBits::eSignaled };

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        imageAvailableSemaphores.emplace_back(logicalDevice.logicalDevice.createSemaphore(semaphoreInfo));
        renderFinishedSemaphores.emplace_back(logicalDevice.logicalDevice.createSemaphore(semaphoreInfo));
        inFlightFences.emplace_back(logicalDevice.logicalDevice.createFence(fenceInfo));
    }
    for (size_t i = 0; i < swapChain.swapChainImages.size(); i++)
    {
        imagesInFlight.emplace_back(nullptr);
    }
}
void vkengine::Renderer::drawLoop()
{
    while (!window.shouldClose())
    {
        glfwPollEvents();
        drawFrame();
    }

    logicalDevice.logicalDevice.waitIdle();
}

void vkengine::Renderer::drawFrame()
{
    if (!valid) {
        refresh_command_buffers();
    }

    while (logicalDevice.logicalDevice.waitForFences(*inFlightFences[currentFrame], VK_TRUE, UINT64_MAX) ==
           vk::Result::eTimeout)
    {
    }

    auto [_, imageIndex] =
      swapChain.swapChain.acquireNextImage(UINT64_MAX, *imageAvailableSemaphores[currentFrame], nullptr);

    if (*imagesInFlight[imageIndex])
    {
        while (logicalDevice.logicalDevice.waitForFences(
                 *imagesInFlight[currentFrame], VK_TRUE, UINT64_MAX) == vk::Result::eTimeout)
        {
        }
    }
    // imagesInFlight[imageIndex] = std::move(inFlightFences[currentFrame]);

    vk::SubmitInfo submitInfo{};
    // submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    vk::Semaphore waitSemaphores[] = { *imageAvailableSemaphores[currentFrame] };
    vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &(*commandBuffers[imageIndex]);

    vk::Semaphore signalSemaphores[] = { *renderFinishedSemaphores[currentFrame] };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    logicalDevice.logicalDevice.resetFences(*inFlightFences[currentFrame]);

    logicalDevice.graphicsQueue.submit(submitInfo, *inFlightFences[currentFrame]);

    vk::PresentInfoKHR presentInfo{};
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    vk::SwapchainKHR swapChains[] = { *swapChain.swapChain };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    auto result = logicalDevice.presentQueue.presentKHR(presentInfo);
    switch (result)
    {
        case vk::Result::eSuccess:
            break;
        case vk::Result::eSuboptimalKHR:
            std::cerr << "vk::Queue::presentKHR returned vk::Result::eSuboptimalKHR !" << std::endl;
            break;
        default:
            assert(false); // an unexpected result is returned !
    }

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void vkengine::Renderer::add_object(const std::vector<Vertex>& vertices)
{
  objects.emplace_back(logicalDevice.logicalDevice, physicalDevice, vertices);
  valid = false;
}
