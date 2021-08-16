//
// Created by nokutu on 16/8/21.
//

#include "object.hpp"
#include <cstring>
#include <stdexcept>
#include <vector>

uint32_t
findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice physicalDevice)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if (
          (typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}

Object::Object(VkDevice device, VkPhysicalDevice physicalDevice, std::vector<Vertex> vertices)
  : _device(device)
  , _vertices(std::move(vertices))
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = sizeof(_vertices[0]) * _vertices.size();
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(_device, &bufferInfo, nullptr, &_vertexBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create vertex buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(_device, _vertexBuffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(
      memRequirements.memoryTypeBits,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
      physicalDevice);

    if (vkAllocateMemory(_device, &allocInfo, nullptr, &_vertexBufferMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate vertex buffer memory!");
    }

    vkBindBufferMemory(_device, _vertexBuffer, _vertexBufferMemory, 0);

    void* data;
    vkMapMemory(_device, _vertexBufferMemory, 0, bufferInfo.size, 0, &data);
    memcpy(data, _vertices.data(), (size_t)bufferInfo.size);
    vkUnmapMemory(_device, _vertexBufferMemory);
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

void Object::bind(VkCommandBuffer& commandBuffer)
{
    VkBuffer vertexBuffers[] = { _vertexBuffer };
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    vkCmdDraw(commandBuffer, static_cast<uint32_t>(_vertices.size()), 1, 0, 0);
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
    if (_device != nullptr)
    {
        vkDestroyBuffer(_device, _vertexBuffer, nullptr);
        vkFreeMemory(_device, _vertexBufferMemory, nullptr);
        clear();
    }
}
