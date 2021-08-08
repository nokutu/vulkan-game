//
// Created by jorge on 8/8/2021.
//

#include "window.hpp"
#include <stdexcept>

void Window::init()
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
}

bool Window::shouldClose()
{
    return glfwWindowShouldClose(window);
}

void Window::pollEvents()
{
    glfwPollEvents();
}

void Window::destroy()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::createSurface(VkInstance& instance)
{
    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create window surface!");
    }
}

std::tuple<int, int> Window::getFramebufferSize()
{
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    return std::make_tuple(width, height);
}
