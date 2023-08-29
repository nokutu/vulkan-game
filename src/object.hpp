//
// Created by nokutu on 16/8/21.
//
#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>

#include "vertex.hpp"

class Object
{
private:
    vk::raii::Buffer _vertexBuffer = nullptr;
    vk::raii::DeviceMemory _vertexBufferMemory = nullptr;

    std::vector<Vertex> _vertices;

public:
    Object() = default;
    explicit Object(const vk::raii::Device& device, const vk::raii::PhysicalDevice& physicalDevice, std::vector<Vertex> vertices);
    Object(Object&& other) noexcept;
    Object& operator=(Object&& other) noexcept;

    void bind(vk::raii::CommandBuffer& commandBuffer);
};
