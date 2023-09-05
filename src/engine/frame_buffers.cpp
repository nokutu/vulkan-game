//
// Created by jorge on 05/09/2023.
//

#include "frame_buffers.hpp"

vkengine::FrameBuffers::FrameBuffers(
  const vk::raii::Device& device,
  const vkengine::SwapChain& swapChain,
  const vkengine::RenderPass& renderPass)

{
    swapChainFramebuffers.reserve(swapChain.swapChainImageViews.size());

    for (size_t i = 0; i < swapChain.swapChainImageViews.size(); i++)
    {
        vk::ImageView attachments[] = { *swapChain.swapChainImageViews[i] };

        vk::FramebufferCreateInfo framebufferInfo{};
        // framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = *renderPass.renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = swapChain.swapChainExtent.width;
        framebufferInfo.height = swapChain.swapChainExtent.height;
        framebufferInfo.layers = 1;

        swapChainFramebuffers.emplace_back(device.createFramebuffer(framebufferInfo));
    }
}
