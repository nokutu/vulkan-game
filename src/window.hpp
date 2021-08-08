//
// Created by jorge on 8/8/2021.
//
#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <tuple>

class Window
{
public:
    void init();
    void createSurface(VkInstance& instance);
    bool shouldClose();
    void pollEvents();
    void destroy();
    std::tuple<int, int> getFramebufferSize();

    VkSurfaceKHR surface;

private:
    static constexpr size_t WIDTH = 800;
    static constexpr size_t HEIGHT = 600;

    GLFWwindow* window;
};
