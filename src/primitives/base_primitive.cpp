//
// Created by jorge on 05/09/2023.
//

#include "base_primitive.hpp"

#include <cstring>
#include <stdexcept>
#include <vector>

std::uint32_t findMemoryType(
  std::uint32_t typeFilter,
  vk::MemoryPropertyFlags properties,
  const vk::raii::PhysicalDevice& physicalDevice)
{
    vk::PhysicalDeviceMemoryProperties memProperties = physicalDevice.getMemoryProperties();

    for (std::uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if (
          (typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}

vkengine::primitives::BasePrimitive::BasePrimitive(
  const vk::raii::Device& device,
  const vk::raii::PhysicalDevice& physicalDevice,
  std::vector<Vertex> vertices)
  : _vertices(std::move(vertices))
{
    vk::BufferCreateInfo bufferInfo(
      vk::BufferCreateFlags(),
      sizeof(_vertices[0]) * _vertices.size(),
      vk::BufferUsageFlagBits::eVertexBuffer,
      vk::SharingMode::eExclusive);
    bufferInfo.sType = vk::StructureType::eBufferCreateInfo;

    _vertexBuffer = device.createBuffer(bufferInfo, nullptr);

    vk::MemoryRequirements memRequirements = _vertexBuffer.getMemoryRequirements();

    vk::MemoryAllocateInfo allocInfo{};
    allocInfo.sType = vk::StructureType::eMemoryAllocateInfo;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(
      memRequirements.memoryTypeBits,
      vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
      physicalDevice);

    _vertexBufferMemory = device.allocateMemory(allocInfo, nullptr);

    _vertexBuffer.bindMemory(*_vertexBufferMemory, 0);

    void* data = _vertexBufferMemory.mapMemory(0, bufferInfo.size, vk::MemoryMapFlags());
    memcpy(data, _vertices.data(), static_cast<std::size_t>(bufferInfo.size));
    _vertexBufferMemory.unmapMemory();
}

vkengine::primitives::BasePrimitive::BasePrimitive(BasePrimitive&& other) noexcept
{
    _vertexBuffer = std::move(other._vertexBuffer);
    _vertexBufferMemory = std::move(other._vertexBufferMemory);

    _vertices = std::move(other._vertices);
}

vkengine::primitives::BasePrimitive& vkengine::primitives::BasePrimitive::operator=(BasePrimitive&& other) noexcept
{
    _vertexBuffer = std::move(other._vertexBuffer);
    _vertexBufferMemory = std::move(other._vertexBufferMemory);

    _vertices = std::move(other._vertices);

    return *this;
}

void vkengine::primitives::BasePrimitive::bind(vk::raii::CommandBuffer& commandBuffer) const
{
    std::array<vk::Buffer, 1> vertexBuffers = { *_vertexBuffer };
    std::array<vk::DeviceSize, 1> offsets = { 0 };
    commandBuffer.bindVertexBuffers(0, vertexBuffers, offsets);
    commandBuffer.draw(static_cast<std::uint32_t>(_vertices.size()), 1, 0, 0);
}
