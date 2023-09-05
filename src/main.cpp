#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <optional>
#include <set>
#include <stdexcept>
#include <vector>

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>

#include "debug.hpp"
#include "engine/frame_buffers.hpp"
#include "engine/graphics_pipeline.hpp"
#include "engine/logical_device.hpp"
#include "engine/physical_device.hpp"
#include "engine/queue_families.hpp"
#include "engine/render_pass.hpp"
#include "engine/swap_chain.hpp"
#include "object.hpp"
#include "renderer.hpp"
#include "shader_manager.hpp"
#include "window.hpp"

const std::vector<Vertex> vertices = { Vertex{ { 0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f } },
                                       Vertex{ { 0.5f, 0.5f }, { 0.0f, 1.0f, 0.0f } },
                                       Vertex{ { 0, 0 }, { 0.0f, 0.0f, 1.0f } } };

const std::vector<Vertex> vertices2 = { Vertex{ { -0.5f, 0.5f }, { 1.0f, 0.0f, 0.0f } },
                                        Vertex{ { -0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f } },
                                        Vertex{ { 0, 0 }, { 0.0f, 0.0f, 1.0f } } };

class HelloTriangleApplication
{
public:
    void start()
    {
        renderer.add_object(vertices);
        renderer.add_object(vertices2);
        renderer.drawLoop();
    }

    HelloTriangleApplication()
      : instance(createInstance())
    {
    }

private:
    vk::raii::Context context;
    vk::raii::Instance instance;
    Window window{ instance };

#ifndef NDEBUG
    DebugMessenger debugMessenger{};
#endif

    vkengine::PhysicalDevice physicalDevice{ instance, window };
    vkengine::LogicalDevice logicalDevice{ instance, window, physicalDevice };
    vkengine::SwapChain swapChain{ *logicalDevice, *physicalDevice, window };
    vkengine::RenderPass renderPass{ *logicalDevice, swapChain };
    vkengine::GraphicsPipeline graphicsPipeline{ *logicalDevice, swapChain, renderPass };
    vkengine::FrameBuffers frameBuffers{ *logicalDevice, swapChain, renderPass };
    vkengine::Renderer renderer{ logicalDevice, *physicalDevice,  window,      swapChain,
                                 renderPass,    graphicsPipeline, frameBuffers };

    vk::raii::Instance createInstance()
    {
        glfwInit();

#ifndef NDEBUG
        if (!checkValidationLayerSupport())
        {
            throw std::runtime_error("validation layers requested, but not available!");
        }
#endif

        vk::ApplicationInfo appInfo{};
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        vk::InstanceCreateInfo createInfo{};
        // createInfo.sType = vk::StructureType::eInstanceCreateInfo;
        createInfo.pApplicationInfo = &appInfo;

        auto extensions = getRequiredExtensions();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

        vk::DebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
#ifndef NDEBUG
        createInfo.enabledLayerCount = static_cast<uint32_t>(vkengine::VALIDATION_LAYERS.size());
        createInfo.ppEnabledLayerNames = vkengine::VALIDATION_LAYERS.data();

        DebugMessenger::populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = &debugCreateInfo;
#else
        createInfo.enabledLayerCount = 0;

        createInfo.pNext = nullptr;
#endif
        return { context, createInfo };
    }

    std::vector<const char*> getRequiredExtensions()
    {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        auto error = glfwGetError(nullptr);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

#ifndef NDEBUG
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

        return extensions;
    }

    bool checkValidationLayerSupport()
    {
        std::vector<vk::LayerProperties> availableLayers = vk::enumerateInstanceLayerProperties();

        for (const char* layerName : vkengine::VALIDATION_LAYERS)
        {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers)
            {
                if (strcmp(layerName, layerProperties.layerName) == 0)
                {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound)
            {
                return false;
            }
        }

        return true;
    }
};

int main()
{
    HelloTriangleApplication app;

    try
    {
        app.start();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
