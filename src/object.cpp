//
// Created by nokutu on 16/8/21.
//

#include "object.hpp"
#include <cstring>
#include <stdexcept>
#include <vector>

std::uint32_t findMemoryType(
  std::uint32_t typeFilter,
  vk::MemoryPropertyFlags properties,
  vk::PhysicalDevice physicalDevice)
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

Object::Object(vk::Device device, vk::PhysicalDevice physicalDevice, std::vector<Vertex> vertices)
  : _device(device)
  , _vertices(std::move(vertices))
{
    vk::BufferCreateInfo bufferInfo(
      vk::BufferCreateFlags(),
      sizeof(_vertices[0]) * _vertices.size(),
      vk::BufferUsageFlagBits::eVertexBuffer,
      vk::SharingMode::eExclusive);
    bufferInfo.sType = vk::StructureType::eBufferCreateInfo;

    _vertexBuffer = _device.createBuffer(bufferInfo, nullptr);

    vk::MemoryRequirements memRequirements = _device.getBufferMemoryRequirements(_vertexBuffer);

    vk::MemoryAllocateInfo allocInfo{};
    allocInfo.sType = vk::StructureType::eMemoryAllocateInfo;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(
      memRequirements.memoryTypeBits,
      vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
      physicalDevice);

    _vertexBufferMemory = _device.allocateMemory(allocInfo, nullptr);

    _device.bindBufferMemory(_vertexBuffer, _vertexBufferMemory, 0);

    void* data = _device.mapMemory(_vertexBufferMemory, 0, bufferInfo.size, vk::MemoryMapFlags());
    memcpy(data, _vertices.data(), static_cast<std::size_t>(bufferInfo.size));
    _device.unmapMemory(_vertexBufferMemory);
}

Object::Object(Object&& other) noexcept
{
    destroy();

    _device = other._device;
    _vertexBuffer = other._vertexBuffer;
    _vertexBufferMemory = other._vertexBufferMemory;

    _vertices = std::move(other._vertices);
    other.clear();
}

Object& Object::operator=(Object&& other) noexcept
{
    destroy();

    _device = other._device;
    _vertexBuffer = other._vertexBuffer;
    _vertexBufferMemory = other._vertexBufferMemory;

    _vertices = std::move(other._vertices);

    other.clear();
    return *this;
}

Object::~Object()
{
    destroy();
}

void Object::bind(vk::CommandBuffer& commandBuffer)
{
    vk::Buffer vertexBuffers[] = { _vertexBuffer };
    vk::DeviceSize offsets[] = { 0 };
    commandBuffer.bindVertexBuffers(0, 1, vertexBuffers, offsets);
    commandBuffer.draw(static_cast<std::uint32_t>(_vertices.size()), 1, 0, 0);
}

void Object::clear()
{
    _device = nullptr;
    _vertexBuffer = nullptr;
    _vertexBufferMemory = nullptr;
    _vertices.clear();
}

void Object::destroy()
{
    if (_device)
    {
        _device.destroyBuffer(_vertexBuffer);
        _device.freeMemory(_vertexBufferMemory);
        clear();
    }
}
