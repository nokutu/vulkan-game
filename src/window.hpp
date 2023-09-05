//
// Created by jorge on 8/8/2021.
//
#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <tuple>
#include <vulkan/vulkan_raii.hpp>

class Window
{
public:
    Window(const vk::raii::Instance& instance);
    ~Window();

    bool shouldClose() const;
    void pollEvents();
    std::tuple<int, int> getFramebufferSize() const;

    vk::raii::SurfaceKHR surface = nullptr;

private:
    static constexpr size_t WIDTH = 800;
    static constexpr size_t HEIGHT = 600;

    GLFWwindow* window;
};
