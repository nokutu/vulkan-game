//
// Created by jorge on 8/8/2021.
//
#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <tuple>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>

class Window
{
public:
    void createSurface(const vk::raii::Instance& instance);
    bool shouldClose();
    void pollEvents();
    std::tuple<int, int> getFramebufferSize();

    vk::raii::SurfaceKHR surface = nullptr;

    Window();
    ~Window();

private:
    static constexpr size_t WIDTH = 800;
    static constexpr size_t HEIGHT = 600;

    GLFWwindow* window;
};
