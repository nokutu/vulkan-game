//
// Created by nokutu on 16/8/21.
//
#pragma once

#include <vector>

#include <vulkan/vulkan_core.h>

#include "vertex.hpp"

class Object
{
private:
    VkDevice _device = nullptr;

    VkBuffer _vertexBuffer = nullptr;
    VkDeviceMemory _vertexBufferMemory = nullptr;

    std::vector<Vertex> _vertices;

    void clear();

public:
    Object() = default;
    explicit Object(VkDevice device, VkPhysicalDevice physicalDevice, std::vector<Vertex> vertices);
    Object(Object&& other) noexcept;
    Object& operator=(Object&& other) noexcept;

    void bind(VkCommandBuffer& commandBuffer);
    void destroy();

    ~Object();
};
