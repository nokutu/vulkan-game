//
// Created by jorge on 8/8/2021.
//

#include "window.hpp"

#include <stdexcept>

#include <fmt/core.h>

Window::Window()
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
}

Window::~Window()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool Window::shouldClose()
{
    return glfwWindowShouldClose(window);
}

void Window::pollEvents()
{
    glfwPollEvents();
}

void Window::createSurface(const vk::raii::Instance& instance)
{
    VkResult result;
    VkSurfaceKHR ptr;
    if ((result = glfwCreateWindowSurface(*instance, window, nullptr, &ptr)) != VK_SUCCESS)
    {
        throw std::runtime_error(fmt::format("Error creating GLFW window: {}", static_cast<int>(result)));
    }
    surface = vk::raii::SurfaceKHR(instance, ptr);
}

std::tuple<int, int> Window::getFramebufferSize()
{
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    return std::make_tuple(width, height);
}
