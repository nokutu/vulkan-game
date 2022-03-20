//
// Created by nokutu on 16/8/21.
//
#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>

#include "vertex.hpp"

class Object
{
private:
    vk::Device _device = nullptr;
    vk::Buffer _vertexBuffer = nullptr;
    vk::DeviceMemory _vertexBufferMemory = nullptr;

    std::vector<Vertex> _vertices;

    void clear();

public:
    Object() = default;
    explicit Object(vk::Device device, vk::PhysicalDevice physicalDevice, std::vector<Vertex> vertices);
    Object(Object&& other) noexcept;
    Object& operator=(Object&& other) noexcept;

    void bind(vk::CommandBuffer& commandBuffer);
    void destroy();

    ~Object();
};
