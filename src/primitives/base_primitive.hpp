//
// Created by jorge on 05/09/2023.
//
#pragma once

#include "../vertex.hpp"

namespace vkengine::primitives
{

class BasePrimitive
{
private:
    vk::raii::Buffer _vertexBuffer = nullptr;
    vk::raii::DeviceMemory _vertexBufferMemory = nullptr;

    std::vector<Vertex> _vertices;

public:
    BasePrimitive() = default;
    BasePrimitive(
      const vk::raii::Device& device,
      const vk::raii::PhysicalDevice& physicalDevice,
      std::vector<Vertex> vertices);

    BasePrimitive(BasePrimitive&& other) noexcept;
    BasePrimitive& operator=(BasePrimitive&& other) noexcept;

    void bind(vk::raii::CommandBuffer& commandBuffer) const;
};
}
