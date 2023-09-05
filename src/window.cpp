//
// Created by jorge on 8/8/2021.
//

#include "window.hpp"

#include <stdexcept>

#include <fmt/core.h>

Window::Window(const vk::raii::Instance& instance)
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);

    VkResult result;
    VkSurfaceKHR ptr;
    if ((result = glfwCreateWindowSurface(*instance, window, nullptr, &ptr)) != VK_SUCCESS)
    {
        throw std::runtime_error(fmt::format("Error creating GLFW window: {}", static_cast<int>(result)));
    }
    surface = vk::raii::SurfaceKHR(instance, ptr);
}

Window::~Window()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool Window::shouldClose() const
{
    return glfwWindowShouldClose(window);
}

void Window::pollEvents()
{
    glfwPollEvents();
}

std::tuple<int, int> Window::getFramebufferSize() const
{
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    return std::make_tuple(width, height);
}
